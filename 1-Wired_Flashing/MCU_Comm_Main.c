#include "STD_TYPES.h"
#include "dGPIO.h"
#include "dRCC.h"
#include "dUART.h"
#include "DNVIC_interface.h"
#include "hLCD.h"
#include "hLCD_Config.h"
#include "dSTK.h"
#include "Sched.h"
#include "HSwitch.h"
#include "HSwitch_Config.h"
#include "TProtocol.h"
#include "TProtocolMessages_Config.h"
#include "hUART_Config.h"


extern const UART_Frame_Cfg_t App_UART_Config[APP_USARTs_NUM];
EraseCommand_t Switchs = {48+8,0};
EraseCommand_t Switchs2;

u8 TrasnmitterBuffer[15] =  {0};
volatile u8 RXBuffer[16] = {0};
u8 May[3]={'M','A','Y'};
u8 MessageID;
u8 x;
u8 CommandFlag =0;

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



void main (void){
	RCC_SetSYSClock(HSE_SW);

	RCC_ControlPerihperal(APB2,RCC_APB2_CLOCK_UART1EN,ON);
	RCC_ControlPerihperal(APB2,RCC_APB2_CLOCK_PORT_A,ON);

	UART_setRecveiverCbf (func);
	UART_Init(&App_UART_Config[0]);

	TProtcol_sendFrame( &Switchs,TrasnmitterBuffer, 0x97);
	UART_ReceiveBuffer(RXBuffer , 1);
	UART_SendBuffer(TrasnmitterBuffer, 14);

	while(1){
		//UART_SendBuffer(TrasnmitterBuffer, PROTOCOL_DATA_BYES);
	}
}


