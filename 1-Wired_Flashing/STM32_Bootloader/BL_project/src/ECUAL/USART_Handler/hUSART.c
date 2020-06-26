#include "STD_TYPES.h"
#include "DNVIC_interface.h"
#include "dGPIO.h"
#include "dRCC.h"
#include "dUART.h"
#include "hUSART.h"
#include "hUART_Config.h"



extern GPIO_t UART_GPIO_TX_Map[NUM_OF_USARTs], UART_GPIO_RX_Map[NUM_OF_USARTs];

STD_ERROR UART_Init(UART_Frame_Cfg_t * cfg)
{
	STD_ERROR Local_ErrorStatus = NOT_OK;
	GPIO_t UART_GPIO_TX, UART_GPIO_RX;

	//TODO: GPIO pin speed to be modified according to the RCC Handler
	// GPIO configure pins ---> could be move to UART HAL
	switch(cfg->UART_ID){
		case UART_ONE:
			RCC_ControlPerihperal(APB2, RCC_APB2_CLOCK_PORT_A,  ON);
			// RCC configuration for UART1
			RCC_ControlPerihperal(APB2, RCC_APB2_CLOCK_UART1EN,  ON);

			break;
		case UART_TWO:
			RCC_ControlPerihperal(APB2, RCC_APB2_CLOCK_PORT_A,  ON);
			// RCC configuration for UART1
			RCC_ControlPerihperal(APB1, RCC_APB1_CLOCK_UART2EN,  ON);

			break;
		case UART_THREE:
			RCC_ControlPerihperal(APB2, RCC_APB2_CLOCK_PORT_B,  ON);
			// RCC configuration for UART1
			RCC_ControlPerihperal(APB1, RCC_APB1_CLOCK_UART3EN,  ON);

			break;
	}

	UART_GPIO_TX = UART_GPIO_TX_Map [cfg->UART_ID];
	UART_GPIO_RX = UART_GPIO_RX_Map [cfg->UART_ID];
	GPIO_Config(&UART_GPIO_TX);
	GPIO_Config(&UART_GPIO_RX);
	GPIO_writePin(UART_GPIO_RX.PORT, UART_GPIO_RX.PIN,  HIGH);


	//Clear pending flag of UART1
	DNVIC_ClearPending(USART1);

	//Enable UART1 IRQ
	DNVIC_EnableIRQ(USART1);

	//Configure UART to the Baudrate, stop bits, Parity bits and interrupts needed
	//UART_Configure( &cfg );
	UART_Configure( cfg );

	return Local_ErrorStatus;
}

ProcessState_t UART_SendBuffer(u8 * buffer, u16 bytesCount)
{
	ProcessState_t Local_RequestState;
	Local_RequestState = UART_Send(buffer,bytesCount);
	return Local_RequestState;
}

ProcessState_t UART_ReceiveBuffer(u8 * buffer, u16 bytesCount)
{
	ProcessState_t Local_RequestState;
	Local_RequestState = UART_Reveive(buffer,bytesCount);
	return Local_RequestState;
}

void UART_setTransmitterCbf (AppTxNotify_t txCbf)
{
	UART_setTxCbf ( txCbf);
}
void UART_setRecveiverCbf(AppRxNotify_t rxCbf)
{
	UART_setRxCbf ( rxCbf );
}

