#include "STD_TYPES.h"
#include "dGPIO.h"
#include "dRCC.h"
#include "dUART.h"
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
void func(void);
#define APP1MARKER 0

u32 AppEntryPoint=0;

extern const UART_Frame_Cfg_t App_UART_Config[APP_USARTs_NUM];
EraseCommand_t Switchs = {48+8,0};
EraseCommand_t Switchs2;

u8 TrasnmitterBuffer[15] =  {0};
volatile u8 RXBuffer[16] = {0};
u8 MessageID;
u8 x;
u8 CommandFlag =0;
u8 Marker;
int main(void)
{
	RCC_SetSYSClock(HSE_SW);

	RCC_ControlPerihperal(APB2,RCC_APB2_CLOCK_UART1EN,ON);
	RCC_ControlPerihperal(APB2,RCC_APB2_CLOCK_PORT_A,ON);

	switch(Marker)
	{
	case APP1MARKER:
		/*new app*/
		break;
	default:
		/*wait to receive new application*/

		break;
	}
	UART_setRecveiverCbf (func);
	UART_Init(&App_UART_Config[0]);
    UART_ReceiveBuffer(RXBuffer , 1);

}

void func(void){

	if (RXBuffer[0]=='A' && CommandFlag == 0){
		UART_ReceiveBuffer( &RXBuffer[1] , 7);
		CommandFlag =1;
	}else if (CommandFlag == 1){
		CommandFlag =0;
		TProtocol_ReceiveFrame( RXBuffer, &Switchs2, &MessageID);
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

