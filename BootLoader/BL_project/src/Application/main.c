
/*====================================================================================================*/
/***********************************************Includes***********************************************/
/*====================================================================================================*/
#include "STD_TYPES.h"
#include "dGPIO.h"
#include "dRCC.h"
#include "dUART.h"
#include "dFlash.h"
#include "hUSART.h"
#include "DNVIC_interface.h"
#include "TProtocol.h"
#include "TProtocolMessages_Config.h"
#include "hUART_Config.h"
#include "DELAY.h"
#include "cmsis_device.h"


/*====================================================================================================*/
/************************************************Macros************************************************/
/*====================================================================================================*/
//#define AppEntryPoint			 0x08003000
#define APP1MARKER      	5
#define STARTOFFRAME    	'A'
#define MAX_DATA_BLOCK  	1000
#define MAINADDRESS     	0x08000000
#define PAGESIZE        	1024
#define MAXERRORCOUNT  	    3
#define Marker          	*(volatile u32 *)(0x0800F618)
#define Marker_Address   	(0x0800F618)


/*====================================================================================================*/
/*****************************************Functions prototypes*****************************************/
/*====================================================================================================*/
static void start_app(uint32_t pc, uint32_t sp) __attribute__((naked));
void newApp(void);


/*====================================================================================================*/
/********************************************Global variables******************************************/
/*====================================================================================================*/
u32 AppEntryPoint = 0x08003000;
DataCommand_t     DataCommand;
EraseCommand_t    EraseCommand;
ResponseCommand_t ResponseCommand;
VerifyCommand_t   VerifyCommand;

extern const UART_Frame_Cfg_t App_UART_Config[APP_USARTs_NUM];

u8 TrasnmitterBuffer[8] = {0};
volatile u8 RXBuffer[8] = {0};
u8 MessageID;

/* TODO : u16 better than u8 */
u8 EraseCheckSum = 0;
u32 DataCheckSum = 0;
u8 CommandFlag = 0;
//u8 Marker=5;
u8 DataBytes[MAX_DATA_BLOCK];
static long DataIterator = 0;
long LastSavedDataIterator = 0;
u8 FrameBytes = 0;
u8 FirstTimeFlag = 1;


int main(void)
{
	
	/* Configuration the System Clk */
	RCC_SetSYSClock(PLL_SW);
	
	/* Configurate the Clock of the some pheripherals */
	RCC_ControlPerihperal(APB2,RCC_APB2_CLOCK_UART1EN,ON);
	RCC_ControlPerihperal(APB2,RCC_APB2_CLOCK_PORT_A,ON);
	RCC_ControlPerihperal(APB2,RCC_APB2_CLOCK_PORT_C,ON);
	
	/* Initalize the UART */
	UART_Init(&App_UART_Config[0]);
	UART_setRecveiverCbf (newApp);
	
	/* Unlock The Flash */
	Flash_Unlock();

	/* Debugging Code */
	GPIO_t	LED={
			.PORT = PORT_C,
			.PIN  = PIN_13,
			.MOOD = OUTPUT_GP_PUSH_PULL_10MHZ
	};
	GPIO_Config(&LED);
	GPIO_writePin(LED.PORT,LED.PIN,LOW);
	Delay_ms(1000);
	GPIO_writePin(LED.PORT,LED.PIN,HIGH);

	while(1)
	{
		/* Check the value of the Marker */
		switch(Marker)
		{

		/* jump to APP1 if exist */
		case APP1MARKER:
		
			/* Set the first time flag */
			FirstTimeFlag = 1;
			
			
			//SCB->VTOR = 0x00003000 << 9;
			
			/* update the Stack Pointer & Program Counter */
			volatile uint32_t *app_code = (uint32_t *)AppEntryPoint;
			volatile uint32_t app_sp = app_code[0];
			volatile uint32_t app_start = app_code[1];
			start_app(app_start, app_sp);
			break;

			/*No APP exists, Waiting to receive new application*/
		default:
		
			/*If the first time flag was set, clear the first time flag,
			  recive one byte via UART to check the start of frame SF*/
			if( FirstTimeFlag == 1 )
			{
				FirstTimeFlag = 0;
				UART_ReceiveBuffer(RXBuffer, 1);
			}
			break;
		}
	}

}

/*====================================================================================================*/
/**********************************Functions Definetion************************************************/
/*====================================================================================================*/


/*====================================================================================================*/
/*************************************Updating SP & PC ************************************************/
/*====================================================================================================*/
static void start_app(uint32_t pc, uint32_t sp)
{
	asm("msr msp, r1 ");
	asm(" bx r0");
}

/*====================================================================================================*/
/***************************************Flashing newApp************************************************/
/*====================================================================================================*/
void newApp(void)
{
	u8 SectionCount = 0;
	u8 ErrorCounter = 0;
	static u8 DataBlock = 0;
	STD_ERROR Local_Error = OK;

	/*If start of frame recived, recive the rest bytes of the frame, set the command flag*/
	if (RXBuffer[0] == STARTOFFRAME && CommandFlag == 0)
	{
		UART_ReceiveBuffer(&RXBuffer[1], 7);
		CommandFlag = 1;
	}

	/*If command flag was set, clear the command flag, check the massage ID in RXBuffer*/
	else if (CommandFlag == 1)
	{
		CommandFlag = 0;

		switch(RXBuffer[1])
		{

		case ID_EraseCommand:

			/*Parse the Erase frame*/
			TProtocol_ReceiveFrame( RXBuffer, &EraseCommand, &MessageID);

			/* TODO : change sectionCounts from u16 to u8 & CheckSum from u8 to u16 */

			/*Calculate the Erase check sum*/
			EraseCheckSum = ( (u8)EraseCommand.SectionsCount  ) + ((u8)EraseCommand.SectionOffset );

			/*if the Erase check sum in the frame parsed is equal to Erase CheckSum that has been calculated*/
			if(EraseCommand.CheckSum == EraseCheckSum )
			{
				/*Wait to Erase page, update the local error variable */
				while(SectionCount < EraseCommand.SectionsCount && ErrorCounter < MAXERRORCOUNT)
				{
					Local_Error = Flash_ErassPage( MAINADDRESS + (EraseCommand.SectionOffset) + ( PAGESIZE * SectionCount) );
					if(Local_Error == OK)
					{
						SectionCount++;
						ErrorCounter=0;
					}
					else
					{
						ErrorCounter++;
					}

				}

				/*if local error is ok, set the response command to OK*/
				if(Local_Error == OK)
				{
					ResponseCommand.Response=R_OK;
				}

				/*else local error is not ok, set the response command to Not ok for EraseFailure*/
				else
				{
					ResponseCommand.Response=R_NOT_EraseFailure;
				}

				/*Clear Erase check sum*/
				EraseCheckSum = 0;

				/*TODO : check the MessageID send to the TP , ID is wrong its an EraseFrame ID instead of a ResponseFrame ID*/

				/* Constuct the Response Frame */
				TProtcol_sendFrame(&ResponseCommand, TrasnmitterBuffer, ID_ResponseCommand);
				
				/* Wait 10000 ms to Sync with the PC */
				Delay_ms(10000);

				/*Send the ID of the response command*/
				UART_SendBuffer(TrasnmitterBuffer, PROTOCOL_DATA_BYTES);
				

			}

			/*else the Erase check sum in the frame parsed is Not equal to Erase CS that has been calculated*/
			else
			{
				/*Set the response command to Not ok for MismatchData*/
				ResponseCommand.Response=R_NOT_MismatchData;

				/* TODO : take the  TProtcol_sendFrame & UART_SendBuffer from here and from above, and place it after this else*/

				/* Constuct the Response Frame */
				TProtcol_sendFrame(&ResponseCommand,TrasnmitterBuffer,ID_ResponseCommand);
				
				/* Wait 10000 ms to Sync with the PC */
				Delay_ms(10000);

				/*Send the ID of the response command*/
				UART_SendBuffer(TrasnmitterBuffer,PROTOCOL_DATA_BYTES);
			}

			/* Receive one byte from the PC */
			UART_ReceiveBuffer(RXBuffer , 1);
			break;

		case ID_DataCommand:

			/*Parse the Data frame*/
			TProtocol_ReceiveFrame( RXBuffer, &DataCommand, &MessageID);

			/* Reset the Local 1000 bytes iterator */
			FrameBytes = 0;

			/*Wait to store The 1000 bytes in local buffer*/
			while(FrameBytes < FRAME_DATA_BYTES)
			{
				DataBytes[DataIterator] = DataCommand.Data[FrameBytes];
				DataCheckSum += DataCommand.Data[FrameBytes];
				
				/* increment Global Iterator */
				DataIterator++;
				
				/* increment local 1000 bytes iterator */
				FrameBytes++;
			}

			/* Receive one byte from the PC */
			UART_ReceiveBuffer(RXBuffer , 1);
			break;

		case ID_VerifyCommand:

			/*Parse the Verify frame*/
			TProtocol_ReceiveFrame( RXBuffer, &VerifyCommand, &MessageID);

			/*If flashing was right by checking the Data check sum that has been calculated is equal to the verfiy command check sum*/
			if(DataCheckSum == VerifyCommand.CheckSum)
			{
				/*Set the responce command to OK */
				ResponseCommand.Response=R_OK;

				/*Clear Data check sum variable */
				DataCheckSum = 0;

				/*Enable PRIMASK interrupts*/
				NVIC_EnablePRIMASK();

				/*Begin Flash the 1000 Received Data Bytes, Clear the data iterator variable, increase the data block variable*/
				Local_Error = Flash_ProgramWrite( MAINADDRESS + (EraseCommand.SectionOffset) + ( MAX_DATA_BLOCK * DataBlock), DataBytes, MAX_DATA_BLOCK);
				DataIterator = 0;
				DataBlock++;

				/*Disable PRIMASK interrupts*/
				NVIC_DisablePRIMASK();
			}
			/*If flashing the data bytes was not right*/
			else
			{
				/*Clear the data check sum and data iterator variable, set response command to NOT OK for MismatchData*/
				DataCheckSum = 0;
				DataIterator=0;
				ResponseCommand.Response=R_NOT_MismatchData;
			}

			/* Constuct the Response Frame */
			TProtcol_sendFrame(&ResponseCommand, TrasnmitterBuffer, ID_ResponseCommand);
			
			/* Wait 10000 ms to Sync with the PC */
			Delay_ms(10000);

			/*Send the ID of the response command*/
			UART_SendBuffer(TrasnmitterBuffer, PROTOCOL_DATA_BYTES);


			/* Check if all The Hex has been received and flashed */
			if(DataBlock == EraseCommand.SectionsCount )
			{
				/* Flash Marker */
				Flash_ErassPage(Marker_Address);
				Flash_WriteWord(&Marker, APP1MARKER);


				/*Clear the data block and the frame bytes variable*/
				DataBlock = 0;
				FrameBytes = 0;

				/*Requesting Software Reset */
				SCB->AIRCR = (0x5FA0000 | ( 1 << 2 ));
			}

			/* Receive one byte from the PC */
			UART_ReceiveBuffer(RXBuffer , 1);
			break;
		}

	}
	else
	{
		/* Receive one byte from the PC */
		UART_ReceiveBuffer(RXBuffer , 1);


	}


}

/*int main()
{
	GPIO_t	LED={
			.PORT = PORT_C,
			.PIN  = PIN_13,
			.MOOD = OUTPUT_GP_PUSH_PULL_10MHZ
	};

	RCC_SetSYSClock(HSE_SW);
	RCC_ControlPerihperal(APB2,RCC_APB2_CLOCK_PORT_C,ON);

	GPIO_Config(&LED);
	GPIO_writePin(LED.PORT,LED.PIN,LOW);

	SCB->VTOR = 0x00003000 << 9;

	volatile uint32_t *app_code = (uint32_t *)AppEntryPoint;
	volatile uint32_t app_sp = app_code[0];
	volatile uint32_t app_start = app_code[1];
	start_app(app_start, app_sp);

//	APP_Entrypoint();  // by this method the program counter is not updated right why ?!!!!!!!!!!!!!!!!!!!

	while(1)
	{

	}
	return 0;
}

 */


/*
 *    APP(rx) : ORIGIN = 0x08003000, LENGTH = 20k
 *
 *
 *    .app : ALIGN(4)
    {

 *(.app .app.*)
        KEEP(*(.app*))


    } >APP
 * */




/* MARKER (rx) : ORIGIN = 0x0800F000, LENGTH = 4
 *
 *
 *
 *     .marker : ALIGN(4)
    {


 *(.marker .marker.*)
        KEEP(*(.marker*))


    } >MARKER
 *
 *
 * */

