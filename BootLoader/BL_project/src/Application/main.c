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

#define APP1MARKER       0
#define STARTOFFRAME    'A'
#define MAX_DATA_BLOCK  750
#define MAINADDRESS     0x08000000
#define PAGESIZE        1024
#define MAXERRORCOUNT   5

void newApp(void);

u32 AppEntryPoint=0;
DataCommand_t DataCommand;
EraseCommand_t EraseCommand;
ResponseCommand_t ResponseCommand;
VerifyCommand_t VerifyCommand;
extern const UART_Frame_Cfg_t App_UART_Config[APP_USARTs_NUM];

u8 TrasnmitterBuffer[8] = {0};
volatile u8 RXBuffer[8] = {0};
u8 MessageID;
u8 x;
long EraseCheckSum;
long DataCheckSum=0;
u8 CommandFlag =0;
u8 Marker=1;
u8 SectionNumber=0;
u8 DataBytes[MAX_DATA_BLOCK];
static long DataIterator=0;
u8 FrameBytes=0;
u8 VerifyIterator=0;

int main(void)
{


	RCC_SetSYSClock(HSE_SW);

	RCC_ControlPerihperal(APB2,RCC_APB2_CLOCK_UART1EN,ON);
	RCC_ControlPerihperal(APB2,RCC_APB2_CLOCK_PORT_A,ON);
	UART_Init(&App_UART_Config[0]);
	UART_setRecveiverCbf (newApp);
	Flash_Unlock();

	while(1)
	{
		switch(Marker)
		{
		case APP1MARKER:
			/*existing app*/
			break;
		default:
			/*wait to receive new application*/
			UART_ReceiveBuffer(RXBuffer , 1);
			break;
		}
	}

}

void newApp(void){

	u8 SectionCount=0;
	u8 ErrorCounter=0;
	STD_ERROR Local_Error=OK;
	/*check start of frame*/
	if (RXBuffer[0]== STARTOFFRAME && CommandFlag == 0){
		UART_ReceiveBuffer( &RXBuffer[1] , 7);
		CommandFlag =1;
	}else if (CommandFlag == 1){
		CommandFlag =0;
		switch(RXBuffer[1])
		{
		case ID_EraseCommand:
			TProtocol_ReceiveFrame( RXBuffer, &EraseCommand, &MessageID);
			EraseCheckSum = ( (u8)EraseCommand.SectionsCount  ) + ((u8)EraseCommand.SectionOffset );
			if(EraseCommand.CheckSum ==EraseCheckSum )
			{
				/*TODO*/

				while(SectionCount <EraseCommand.SectionsCount && ErrorCounter<MAXERRORCOUNT)
				{
					Local_Error=Flash_ErassPage(MAINADDRESS+(EraseCommand.SectionOffset) +( PAGESIZE*SectionCount));
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

				ResponseCommand.Response=R_OK;
				TProtcol_sendFrame(&ResponseCommand,TrasnmitterBuffer,&MessageID);
				UART_SendBuffer(TrasnmitterBuffer,PROTOCOL_DATA_BYES);


			}
			else
			{
				/*TODO*/
			}
			break;
		case ID_DataCommand:
			TProtocol_ReceiveFrame( RXBuffer, &DataCommand, &MessageID);
			FrameBytes=0;
			while(FrameBytes<FRAME_DATA_BYTES)
			{
				DataBytes[DataIterator]=DataCommand.Data[FrameBytes];
				DataIterator++;
				FrameBytes++;
			}
			break;
		case ID_VerifyCommand:
			TProtocol_ReceiveFrame( RXBuffer, &VerifyCommand, &MessageID);

			for(VerifyIterator=0;VerifyIterator<MAX_DATA_BLOCK;VerifyIterator++)
			{
				DataCheckSum+=DataBytes[(DataIterator-MAX_DATA_BLOCK)+VerifyIterator];
			}
			if(DataCheckSum == VerifyCommand.CheckSum)
			{
				/*TODO*/
				ResponseCommand.Response=R_OK;
				TProtcol_sendFrame(&ResponseCommand,TrasnmitterBuffer,&MessageID);
				UART_SendBuffer(TrasnmitterBuffer,PROTOCOL_DATA_BYES);
			}
			else
			{
				/*TODO*/
			}
			break;
		}

	}else{
		UART_ReceiveBuffer(RXBuffer , 1);
	}

	x++;
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

