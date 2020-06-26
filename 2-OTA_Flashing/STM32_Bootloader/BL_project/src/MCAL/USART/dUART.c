/*
 * dUART.c
 *
 *  Created on: Mar 25, 2020
 *      Author: FatmaGomaa
 */
#include "STD_TYPES.h"
#include "dUART.h"


#define USART_SR_TXE							0x00000080
#define USART_SR_RXNE							0x00000020
#define USART_SR_TC								0x00000040

#define USART_DR_DR_M							0x000001FF
#define USART_DR_DR_CLEAR_M						0xFFFFFE00

#define USART_BRR_DIV_MANTISSA_M				0x0000FFF0
#define USART_BRR_DIV_MANTISSA_CLEAR_M			0xFFFF000F

#define USART_BRR_DIV_FRACTION_M				0x0000000F
#define USART_BRR_DIV_FRACTION_CLEAR_M			0xFFFFFFF0

#define USART_BRR_USARTDIV_CLEAR_M				0xFFFF0000
#define USART_BRR_USARTDIV_M					0x0000FFFF

#define USART_CR1_UE                            0x00002000
#define USART_CR1_M                             0x00001000
#define USART_CR1_NUM_OF_DATA_BITS_CLEAR_M      0xFFFFEFFF
#define USART_CR1_PCE                           0x00000400
#define USART_CR1_PS                            0x00000200
#define USART_CR1_PARITY_CLEAR_M				0xFFFFF9FF
#define USART_CR1_TCIE                          0x00000040
#define USART_CR1_TXEIE                         0x00000080
#define USART_CR1_RXEIE                         0x00000020
#define USART_CR1_TE                            0x00000008
#define USART_CR1_RE	                        0x00000004

#define USART_CR2_STOP_M						0x00003000
#define USART_CR2_STOP_CLEAR_M					0xFFFFCFFF

#define UART_BASE_ADDRESS			((volatile void*) 0x40013800)

typedef struct {
	u32 UART_SR;
	u32 UART_DR;
	u32 UART_BRR;
	u32 UART_CR1;
	u32 UART_CR2;
	u32 UART_CR3;
	u32 UART_GTPR;
}UartPeripheral_t;

typedef enum{
	Idle_State ,
	Busy_State
}BufferState_t;

typedef struct{
	u8 * ptr;
	u32 position;
	u32 size;
	BufferState_t state;
}DataBuffer_t;


volatile static UartPeripheral_t * UART = (UartPeripheral_t *) UART_BASE_ADDRESS ;

volatile static DataBuffer_t TxBuffer;
volatile static DataBuffer_t RxBuffer;

volatile static AppTxNotify_t AppTxNotify ;
volatile static AppRxNotify_t AppRxNotify ;

// 10ms
void USART1_IRQHandler(void)
{
	if( (UART->UART_SR && USART_SR_TC) ){
		//clear TXE / TC
		UART->UART_SR &= ~ USART_SR_TC;

		if(TxBuffer.size != TxBuffer.position){
			UART->UART_DR = TxBuffer.ptr[TxBuffer.position];
			TxBuffer.position++;
		} else {
			// transmission Done
			TxBuffer.ptr = NULL ;
			TxBuffer.size = 0 ;
			TxBuffer.position = 0;
			TxBuffer.state = Idle_State ;
			if(AppTxNotify)
				AppTxNotify();
		}
	}

	if((UART->UART_SR && USART_SR_RXNE)){
		// clear RXNE
		//UART->UART_SR &= ~ USART_SR_RXNE;

		//UART->UART_SR &= ~ (0x00000010);

		if(RxBuffer.state == Busy_State){

			RxBuffer.ptr[RxBuffer.position] = (u8)(UART->UART_DR) ;
			// clear RXNE
			//UART->UART_SR &= ~ USART_SR_RXNE;
			RxBuffer.position++;

			if(RxBuffer.position == RxBuffer.size){

				RxBuffer.ptr = NULL ;
				RxBuffer.size = 0 ;
				RxBuffer.position = 0 ;
				RxBuffer.state = Idle_State ;
				if(AppRxNotify)
					AppRxNotify();
			}

		}/*
		else if((UART->UART_SR && (0x00000008))){
			RxBuffer.ptr[RxBuffer.position] = (u8)(UART->UART_DR) ;
		}*/
	}
}
ProcessState_t UART_Send(u8 * buffer, u16 bytesCount)
{
	ProcessState_t Local_RequestState = STATE_NOK ;

	u32 Local_u32PeripheralTemp;

	// setup interrupts ---> Transmitting  enable interrupt
	Local_u32PeripheralTemp = UART->UART_CR1;
	Local_u32PeripheralTemp &= ~  (USART_CR1_TCIE);
	Local_u32PeripheralTemp |= (USART_CR1_TCIE); //| USART_CR1_TXEIE | USART_CR1_RXEIE);
	UART->UART_CR1			= Local_u32PeripheralTemp;


	if(buffer && (bytesCount > 0) ){
		if(TxBuffer.state == Idle_State){
			UART->UART_CR1 |= USART_CR1_TE;
			TxBuffer.state = Busy_State ;
			TxBuffer.ptr = buffer ;
			TxBuffer.position = 0 ;
			TxBuffer.size = bytesCount ;

			//UART->UART_DR = 'm' ;
			UART->UART_DR = TxBuffer.ptr[TxBuffer.position];
			TxBuffer.position++;

			Local_RequestState = STATE_OK;
		}else {
			Local_RequestState = STATE_BUSY;
		}
	}else {
		Local_RequestState = STATE_NOK;
	}

	return Local_RequestState;
}

ProcessState_t UART_Reveive(u8 * buffer, u16 bytesCount)
{
	ProcessState_t Local_RequestState ;
	u32 Local_u32PeripheralTemp;

	// setup interrupts ---> Receiving enable interrupt
	Local_u32PeripheralTemp = UART->UART_CR1;
	Local_u32PeripheralTemp &= ~  (USART_CR1_RXEIE);
	Local_u32PeripheralTemp |= (USART_CR1_RXEIE); //| USART_CR1_TXEIE | USART_CR1_RXEIE);
	UART->UART_CR1			= Local_u32PeripheralTemp;

	if(buffer && (bytesCount > 0)){
		if(	RxBuffer.state == Idle_State){
			UART->UART_CR1 |= USART_CR1_RE;
			RxBuffer.ptr = buffer ;
			RxBuffer.size = bytesCount;
			RxBuffer.position = 0;
			RxBuffer.state = Busy_State;
			Local_RequestState = STATE_OK ;
		} else {
			Local_RequestState = STATE_BUSY ;
		}
	}else{
		Local_RequestState = STATE_NOK;
	}
	return Local_RequestState;
}

void UART_Configure( UART_Frame_Cfg_t * cfg )
{
	u32 Local_u32PeripheralTemp =0;

	//1- Enable the USART by writing the UE bit in USART_CR1 register to 1.
	// setup interrupts ---> enable interrupt
	Local_u32PeripheralTemp = UART->UART_CR1;
	Local_u32PeripheralTemp &= ~ (USART_CR1_UE); // | USART_CR1_TXEIE | USART_CR1_RXEIE);
	Local_u32PeripheralTemp |= (USART_CR1_UE); //| USART_CR1_TXEIE | USART_CR1_RXEIE);
	UART->UART_CR1			= Local_u32PeripheralTemp;

	//2- Program the M bit in USART_CR1 to define the word length.
	Local_u32PeripheralTemp =  UART->UART_CR1;
	Local_u32PeripheralTemp &= USART_CR1_NUM_OF_DATA_BITS_CLEAR_M;
	Local_u32PeripheralTemp |= cfg->UART_NumOfDataBits;
	UART->UART_CR1 = Local_u32PeripheralTemp;

	//3- Program the number of stop bits in USART_CR2.
	Local_u32PeripheralTemp = UART->UART_CR2;
	Local_u32PeripheralTemp &= USART_CR2_STOP_CLEAR_M;
	Local_u32PeripheralTemp |= cfg->UART_StopBits;
	UART->UART_CR2 = Local_u32PeripheralTemp;

	//5- Select the desired baud rate using the USART_BRR register.
	UART->UART_BRR = cfg->UART_Baudrate;

	Local_u32PeripheralTemp = UART->UART_CR1;
	Local_u32PeripheralTemp &= USART_CR1_PARITY_CLEAR_M;
	Local_u32PeripheralTemp |= cfg->UART_ParityBits;
	UART->UART_CR1 = Local_u32PeripheralTemp;
}


void UART_setTxCbf (AppTxNotify_t txCbf)
{
	if(txCbf){
		AppTxNotify = txCbf;
	}
}
void UART_setRxCbf(AppRxNotify_t rxCbf)
{
	if(rxCbf){
		AppRxNotify = rxCbf;
	}
}




