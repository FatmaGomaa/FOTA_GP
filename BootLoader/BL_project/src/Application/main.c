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


static void start_app(uint32_t pc, uint32_t sp) __attribute__((naked));

static void start_app(uint32_t pc, uint32_t sp)
{
	asm("msr msp, r1 ");
	asm(" bx r0");
}

//#define AppEntryPoint			 0x08003000

#define APP1MARKER       5
#define STARTOFFRAME    'A'
#define MAX_DATA_BLOCK  1000
#define MAINADDRESS     0x08000000
#define PAGESIZE        1024
#define MAXERRORCOUNT   3


#define  Marker          *(volatile u32 *)(0x0800F618)
#define  Marker_Address   (0x0800F618)


void newApp(void);

u32 AppEntryPoint=0x08003000;
DataCommand_t DataCommand;
EraseCommand_t EraseCommand;
ResponseCommand_t ResponseCommand;
VerifyCommand_t VerifyCommand;
extern const UART_Frame_Cfg_t App_UART_Config[APP_USARTs_NUM];

u8 TrasnmitterBuffer[8] = {0};
volatile u8 RXBuffer[8] = {0};
u8 MessageID;

/* TODO : u16 better than u8 */
u8 EraseCheckSum=0;
u32 DataCheckSum=0;
u8 CommandFlag =0;

//u8 Marker=5;
u8 DataBytes[MAX_DATA_BLOCK];
static long DataIterator=0;
long LastSavedDataIterator=0;
u8 FrameBytes=0;
u8 FirstTimeFlag=1;
u8 startOfReceptionFlag=0;

int main(void)
{

	RCC_SetSYSClock(PLL_SW);

	RCC_ControlPerihperal(APB2,RCC_APB2_CLOCK_UART1EN,ON);
	RCC_ControlPerihperal(APB2,RCC_APB2_CLOCK_PORT_A,ON);
	RCC_ControlPerihperal(APB2,RCC_APB2_CLOCK_PORT_C,ON);
	UART_Init(&App_UART_Config[0]);
	UART_setRecveiverCbf (newApp);
	Flash_Unlock();

//Flash_ErassPage(Marker_Address);
	//Flash_WriteWord(&Marker, 1);

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
		switch(Marker)
		{
		case APP1MARKER:
			/*existing app*/
			FirstTimeFlag=1;
			//SCB->VTOR = 0x00003000 << 9;


			volatile uint32_t *app_code = (uint32_t *)AppEntryPoint;
			volatile uint32_t app_sp = app_code[0];
			volatile uint32_t app_start = app_code[1];

			start_app(app_start, app_sp);
			break;
		default:
			/*wait to receive new application*/
			if( FirstTimeFlag == 1 )
			{
				FirstTimeFlag=0;
				UART_ReceiveBuffer(RXBuffer , 1);
			}

			break;
		}
	}

}

void newApp(void){

	u8 SectionCount=0;
	u8 ErrorCounter=0;
	static u8 DataBlock=0;
	STD_ERROR Local_Error=OK;
	/*check start of frame*/
	if (RXBuffer[0]== STARTOFFRAME && CommandFlag == 0){
		UART_ReceiveBuffer( &RXBuffer[1] , 7);
		CommandFlag =1;
		//startOfReceptionFlag=1;
	}else if (CommandFlag == 1){
		CommandFlag =0;
		switch(RXBuffer[1])
		{
		case ID_EraseCommand:
			TProtocol_ReceiveFrame( RXBuffer, &EraseCommand, &MessageID);

			/* TODO : change sectionCounts from u16 to u8 & CheckSum from u8 to u16 */
			EraseCheckSum = ( (u8)EraseCommand.SectionsCount  ) + ((u8)EraseCommand.SectionOffset );
			if(EraseCommand.CheckSum == EraseCheckSum )
			{

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
				if(Local_Error == OK)
				{
					ResponseCommand.Response=R_OK;
				}
				else
				{
					ResponseCommand.Response=R_NOT_EraseFailure;
				}
				EraseCheckSum = 0;
				/*TODO : check the MessageID send to the TP , ID is wrong its an EraseFrame ID instead of a ResponseFrame ID*/
				TProtcol_sendFrame(&ResponseCommand, TrasnmitterBuffer, ID_ResponseCommand);
				Delay_ms(10000);
				UART_SendBuffer(TrasnmitterBuffer, PROTOCOL_DATA_BYES);

			}
			else
			{

				ResponseCommand.Response=R_NOT_MismatchData;

				/* TODO : take the  TProtcol_sendFrame & UART_SendBuffer from here and from above, and place it after this else*/
				TProtcol_sendFrame(&ResponseCommand,TrasnmitterBuffer,ID_ResponseCommand);
				Delay_ms(10000);
				UART_SendBuffer(TrasnmitterBuffer,PROTOCOL_DATA_BYES);
			}
			UART_ReceiveBuffer(RXBuffer , 1);
			break;
		case ID_DataCommand:
			TProtocol_ReceiveFrame( RXBuffer, &DataCommand, &MessageID);
			FrameBytes=0;
			while(FrameBytes < FRAME_DATA_BYTES)
			{
				DataBytes[DataIterator] = DataCommand.Data[FrameBytes];
				DataCheckSum+=DataCommand.Data[FrameBytes];
				DataIterator++;
				FrameBytes++;
			}
			UART_ReceiveBuffer(RXBuffer , 1);
			break;
		case ID_VerifyCommand:
			TProtocol_ReceiveFrame( RXBuffer, &VerifyCommand, &MessageID);
			if(DataCheckSum == VerifyCommand.CheckSum)
			{
				ResponseCommand.Response=R_OK;
				DataCheckSum = 0;
				NVIC_EnablePRIMASK();
				Local_Error = Flash_ProgramWrite( MAINADDRESS + (EraseCommand.SectionOffset) + ( MAX_DATA_BLOCK * DataBlock), DataBytes, MAX_DATA_BLOCK);
				DataIterator = 0;
				DataBlock++;

				NVIC_DisablePRIMASK();
			}
			else
			{
				DataCheckSum = 0;
				DataIterator=0;
				ResponseCommand.Response=R_NOT_MismatchData;
			}
			TProtcol_sendFrame(&ResponseCommand, TrasnmitterBuffer, ID_ResponseCommand);
			Delay_ms(10000);
			UART_SendBuffer(TrasnmitterBuffer, PROTOCOL_DATA_BYES);

			if(DataBlock == EraseCommand.SectionsCount )
			{
				/*TODO: Soft Reset after updating Marker*/
				/*flash marker*/
				Flash_ErassPage(Marker_Address);
				Flash_WriteWord(&Marker, APP1MARKER);
				//Marker = APP1MARKER;
				DataBlock = 0;
				FrameBytes = 0;
				/*Requesting Software Reset */

				SCB->AIRCR = (0x5FA0000 | ( 1 << 2 ));
			}

			UART_ReceiveBuffer(RXBuffer , 1);
			break;
		}

	}else{
		//if(startOfReceptionFlag == 0){
		UART_ReceiveBuffer(RXBuffer , 1);

		//}
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

