/*
 * dUART_Config.c
 *
 *  Created on: Mar 26, 2020
 *      Author: FatmaGomaa
 */
#include "STD_TYPES.h"
#include "dGPIO.h"
#include "dRCC.h"
#include "dUART.h"
#include "hUART_Config.h"


#define UART_PARITY_EXIST_ODD             				0x00000600
#define UART_PARITY_EXIST_EVEN             				0x00000400
#define UART_PARITY_DOES_NOT_EXIST						0x00000000

#define UART_STOP_BIT_ONE								0x00000000
#define UART_STOP_BIT_TWO								0x00002000
/*341 old value**/
#define UART_BAUD_RATE_9600								0x00000351
#define UART_BAUD_RATE_115200							0x00000045
#define UART_BAUD_RATE_9600_PLL							0x00001D4C
#define UART_BAUD_RATE_115200_PLL						0x00000271
#define UART_BAUD_RATE_230400_PLL						0x00000136
#define UART_BAUD_RATE_500000_PLL						0x00000090

#define USART_ONE					UART_ONE
#define USART_TWO					UART_TWO
#define USART_THREE					UART_THREE

const UART_Frame_Cfg_t App_UART_Config[APP_USARTs_NUM] = {
		{
			.UART_ID			= USART_ONE,
			.UART_Baudrate 		= UART_BAUD_RATE_500000_PLL,
			.UART_ParityBits 	= UART_PARITY_DOES_NOT_EXIST,
			.UART_StopBits	 	= UART_STOP_BIT_ONE
		}
};

GPIO_t UART_GPIO_TX_Map[NUM_OF_USARTs]={
		{
			   .PORT = PORT_A,
			   .PIN  = PIN_9,
			   .MOOD = OUTPUT_AF_PUSH_PULL_10MHZ

		},
		{
		        .PORT = PORT_A,
		        .PIN  = PIN_2,
		        .MOOD = OUTPUT_AF_PUSH_PULL_10MHZ
		},
		{
		        .PORT = PORT_B,
		        .PIN  = PIN_10,
		        .MOOD = OUTPUT_AF_PUSH_PULL_10MHZ
		}

};

GPIO_t UART_GPIO_RX_Map[NUM_OF_USARTs]={
		{
				.PORT = PORT_A,
				.PIN  = PIN_10,
				.MOOD = INPUT_PULLUP_PULLDOWN_MODE
		},
		{
				.PORT = PORT_A,
				.PIN  = PIN_3,
				.MOOD = INPUT_PULLUP_PULLDOWN_MODE
		},
		{
		        .PORT = PORT_B,
		        .PIN  = PIN_11,
		        .MOOD = OUTPUT_AF_PUSH_PULL_10MHZ
		}

};

