/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * File name: GPIO_program.c                                                                                                   *
 *                                                                                                                             *
 * Description: This source file is used to implement interfacing with the GPIO module in stm32f10x evaluation kit             *
 *                                                                                                                             *
 *                                                                                                                             *
 * Author : Mohamed Adel Anis Ahmed				                                                                               *
 * date   : 15 May 2020                                                                                                      *
 * version: 2.0                                                                                                                *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/*====================================================================================================*/
/***********************************************Includes***********************************************/
/*====================================================================================================*/
#include "STD_TYPES.h"

#include "GPIO_interface.h"

/*====================================================================================================*/
/************************************************Macros************************************************/
/*====================================================================================================*/
#define MAX_HALF_PORT_VALUE		(256)
#define MAX_HALF_NUMBER_BITS	(8)
#define MAX_NUMBER_BITS			(16)
#define MODE_CLEAR				(0b1111)

/*====================================================================================================*/
/*********************************************Definitions**********************************************/
/*====================================================================================================*/

/* this structure contains all the registers of the GPIO peripheral */
typedef struct{

	u32 GPIO_CRL;
	u32 GPIO_CRH;
	u32 GPIO_IDR;
	u32 GPIO_ODR;
	u32 GPIO_BSRR;
	u32 GPIO_BRR;
	u32 GPIO_LCKR;

}PortRegister_t;

/*====================================================================================================*/
/*****************************************APIs Implementation******************************************/
/*====================================================================================================*/

GPIO_ErrorStatus_t GPIO_InitStr(GPIO_PinConfig_t* GPIO_PinConfig)
{
	volatile PortRegister_t* PortRegisterPtr;
	PortRegisterPtr = (PortRegister_t*) GPIO_PinConfig->PORT;

	/* creating local Error to return the status of the function after finishing */
	GPIO_ErrorStatus_t LocalError = GPIO_ERR_OK;

	/* check if the user entered valid port */
	if( (GPIO_PinConfig->PORT != PORTA)  && (GPIO_PinConfig->PORT != PORTB) && (GPIO_PinConfig->PORT != PORTC) )
	{
		LocalError = GPIO_ERR_NOK;
	}
	else
	{
		if( GPIO_PinConfig->PIN <= 7 )
		{
			PortRegisterPtr->GPIO_CRL &= ~( MODE_CLEAR << (4 * GPIO_PinConfig->PIN) );

			if(GPIO_PinConfig->MODE == MODE_INPUT_PULLUP)
			{
				PortRegisterPtr->GPIO_CRL |= ( MODE_INPUT_PULLUP << (4 * GPIO_PinConfig->PIN) );
				PortRegisterPtr->GPIO_BSRR = ( 1 << GPIO_PinConfig->PIN );
			}
			else if(GPIO_PinConfig->MODE == MODE_INPUT_PULLDOWN)
			{
				PortRegisterPtr->GPIO_CRL |= ( (MODE_INPUT_PULLUP) << (4 * GPIO_PinConfig->PIN) );
				PortRegisterPtr->GPIO_BRR  = ( 1 << GPIO_PinConfig->PIN );
			}
			else
			{
				PortRegisterPtr->GPIO_CRL |= ( GPIO_PinConfig->MODE << (4 * GPIO_PinConfig->PIN) );
			}

		}
		else if( GPIO_PinConfig->PIN <= 15 )
		{
			(PortRegisterPtr->GPIO_CRH) &= ~( MODE_CLEAR << (4 * (GPIO_PinConfig->PIN - 8)) );



			if(GPIO_PinConfig->MODE == MODE_INPUT_PULLUP)
			{
				PortRegisterPtr->GPIO_CRH |= ( MODE_INPUT_PULLUP << (4 * (GPIO_PinConfig->PIN - 8)) );
				PortRegisterPtr->GPIO_BSRR = ( 1 << GPIO_PinConfig->PIN );
			}
			else if(GPIO_PinConfig->MODE == MODE_INPUT_PULLDOWN)
			{
				PortRegisterPtr->GPIO_CRH |= ( MODE_INPUT_PULLUP << (4 * (GPIO_PinConfig->PIN - 8)) );
				PortRegisterPtr->GPIO_BRR  = ( 1 << GPIO_PinConfig->PIN );
			}
			else
			{
				PortRegisterPtr->GPIO_CRH |= (GPIO_PinConfig->MODE << (4 * (GPIO_PinConfig->PIN - 8)) );
			}

		}
		else
		{
			LocalError = GPIO_ERR_NOK;
		}
	}

return LocalError;

}




GPIO_ErrorStatus_t GPIO_SetPinValue(GPIO_PinConfig_t* GPIO_PinConfig , uint8_t Value)
{
	volatile PortRegister_t* PortRegisterPtr;
	PortRegisterPtr = (PortRegister_t*) GPIO_PinConfig->PORT;

	/* creating local Error to return the status of the function after finishing */
	GPIO_ErrorStatus_t LocalError = GPIO_ERR_OK;

	/* check if the user entered valid port */
	if( (GPIO_PinConfig->PORT != PORTA)  && (GPIO_PinConfig->PORT != PORTB) && (GPIO_PinConfig->PORT != PORTC) )
	{
		LocalError = GPIO_ERR_NOK;
	}
	else
	{
		switch(Value)
		{
			case HIGH:
					PortRegisterPtr->GPIO_BSRR = (1 << GPIO_PinConfig->PIN);
				break;

			case LOW:
					PortRegisterPtr->GPIO_BRR  = (1 << GPIO_PinConfig->PIN);
				break;
		}
	}
	return LocalError;
}




GPIO_ErrorStatus_t GPIO_GetPinValue(GPIO_PinConfig_t* GPIO_PinConfig , uint8_t* Value)
{
	volatile PortRegister_t* PortRegisterPtr;
	PortRegisterPtr = (PortRegister_t*) GPIO_PinConfig->PORT;

	/* creating local Error to return the status of the function after finishing */
	GPIO_ErrorStatus_t LocalError = GPIO_ERR_OK;

	/* check if the user entered valid port */
	if( (GPIO_PinConfig->PORT != PORTA)  && (GPIO_PinConfig->PORT != PORTB) && (GPIO_PinConfig->PORT != PORTC) )
	{
		LocalError = GPIO_ERR_NOK;
	}
	else
	{
		if( (PortRegisterPtr->GPIO_IDR & (1 << GPIO_PinConfig->PIN) ) == 0)
		{
			*Value = LOW;
		}
		else
		{
			*Value = HIGH;
		}
	}

	return LocalError;
}










GPIO_ErrorStatus_t GPIO_InitPins(void* PORT , uint16_t P_NUMs , uint8_t MODE)
{
	volatile PortRegister_t* PortRegisterPtr;
	PortRegisterPtr = (PortRegister_t*) PORT;

	/* creating local Error to return the status of the function after finishing */
	GPIO_ErrorStatus_t LocalError = GPIO_ERR_OK;

	/* check if the user entered valid port */
	if( (PORT != PORTA)  && (PORT != PORTB) && (PORT != PORTC) )
	{
		LocalError = GPIO_ERR_NOK;
	}
	else
	{
		/*check if you are accessing only the lowest half of the port*/
		if( P_NUMs < MAX_HALF_PORT_VALUE )
		{
			for(uint8_t index = 0 ; index < MAX_HALF_NUMBER_BITS ; index++)
			{
				if( ((P_NUMs >> index) & HIGH) == HIGH)
				{
					PortRegisterPtr->GPIO_CRL &= ~( MODE_CLEAR << (4*index) );

					if(MODE == MODE_INPUT_PULLUP)
					{
						PortRegisterPtr->GPIO_CRL  |= ( MODE_INPUT_PULLUP << (4 * index) );
						PortRegisterPtr->GPIO_BSRR  = ( 1 << index );
					}
					else if(MODE == MODE_INPUT_PULLDOWN)
					{
						PortRegisterPtr->GPIO_CRL |= ( MODE_INPUT_PULLUP << (4 * index) );
						PortRegisterPtr->GPIO_BRR  = ( 1 << index );
					}
					else
					{
						PortRegisterPtr->GPIO_CRL |= ( MODE << (4 * index) );
					}
				}
			}
		}
		else
		{
			for(uint8_t index = 0 ; index < MAX_HALF_NUMBER_BITS ; index++)
			{
				if( ((P_NUMs >> index) & HIGH) == HIGH)
				{
					PortRegisterPtr->GPIO_CRL &= ~( MODE_CLEAR << (4*index) );

					if(MODE == MODE_INPUT_PULLUP)
					{
						PortRegisterPtr->GPIO_CRL |= ( MODE_INPUT_PULLUP << (4 * index) );
						PortRegisterPtr->GPIO_BSRR = ( 1 << index );
					}
					else if(MODE == MODE_INPUT_PULLDOWN)
					{
						PortRegisterPtr->GPIO_CRL |= ( MODE_INPUT_PULLUP << (4 * index) );
						PortRegisterPtr->GPIO_BRR  = ( 1 << index );
					}
					else
					{
						PortRegisterPtr->GPIO_CRL |= ( MODE << (4 * index) );
					}
				}
			}
			for(uint8_t index = MAX_HALF_NUMBER_BITS ; index < MAX_NUMBER_BITS ; index++)
			{
				if( ((P_NUMs >> index) & HIGH) == HIGH)
				{
					PortRegisterPtr->GPIO_CRH &= ~( MODE_CLEAR << (4*(index-8)) );

					if(MODE == MODE_INPUT_PULLUP)
					{
						PortRegisterPtr->GPIO_CRH |= ( MODE_INPUT_PULLUP << (4 * (index-8)) );
						PortRegisterPtr->GPIO_BSRR = ( 1 << index );
					}
					else if(MODE == MODE_INPUT_PULLDOWN)
					{
						PortRegisterPtr->GPIO_CRH |= ( MODE_INPUT_PULLUP << (4 * (index-8)) );
						PortRegisterPtr->GPIO_BRR  = ( 1 << index );
					}
					else
					{
						PortRegisterPtr->GPIO_CRH |= ( MODE << (4 * (index-8)) );
					}
				}
			}

		}
	}

	return LocalError;
}





GPIO_ErrorStatus_t GPIO_InitOnePin(void* PORT , uint8_t PIN , uint8_t MODE)
{
	volatile PortRegister_t* PortRegisterPtr;
	PortRegisterPtr = (PortRegister_t*) PORT;

	/* creating local Error to return the status of the function after finishing */
	GPIO_ErrorStatus_t LocalError = GPIO_ERR_OK;

	/* check if the user entered valid port */
	if( (PORT != PORTA)  && (PORT != PORTB) && (PORT != PORTC) )
	{
		LocalError = GPIO_ERR_NOK;
	}
	else
	{
		if( PIN <= 7 )
			{
				PortRegisterPtr->GPIO_CRL &= ~( MODE_CLEAR << (4 * PIN) );

				if(MODE == MODE_INPUT_PULLUP)
				{
					PortRegisterPtr->GPIO_CRL |= ( MODE_INPUT_PULLUP << (4 * PIN) );
					PortRegisterPtr->GPIO_BSRR = ( 1 << PIN );
				}
				else if(MODE == MODE_INPUT_PULLDOWN)
				{
					PortRegisterPtr->GPIO_CRL |= ( (MODE_INPUT_PULLUP) << (4 * PIN) );
					PortRegisterPtr->GPIO_BRR  = ( 1 << PIN );
				}
				else
				{
					PortRegisterPtr->GPIO_CRL |= ( MODE << (4 * PIN) );
				}

			}
			else if( PIN <= 15 )
			{
				(PortRegisterPtr->GPIO_CRH) &= ~( MODE_CLEAR << (4 * (PIN - 8)) );



				if(MODE == MODE_INPUT_PULLUP)
				{
					PortRegisterPtr->GPIO_CRH |= ( MODE_INPUT_PULLUP << (4 * (PIN - 8)) );
					PortRegisterPtr->GPIO_BSRR = ( 1 << PIN );
				}
				else if(MODE == MODE_INPUT_PULLDOWN)
				{
					PortRegisterPtr->GPIO_CRH |= ( MODE_INPUT_PULLUP << (4 * (PIN - 8)) );
					PortRegisterPtr->GPIO_BRR  = ( 1 << PIN );
				}
				else
				{
					PortRegisterPtr->GPIO_CRH |= ( MODE << (4 * (PIN - 8)) );
				}

			}
			else
			{
				LocalError = GPIO_ERR_NOK;
			}
	}

	return LocalError;

}




GPIO_ErrorStatus_t GPIO_writePinValue(void* PORT , uint16_t P_NUM , uint8_t Value)
{
	volatile PortRegister_t* PortRegisterPtr;
	PortRegisterPtr = (PortRegister_t*) PORT;

	/* creating local Error to return the status of the function after finishing */
	GPIO_ErrorStatus_t LocalError = GPIO_ERR_OK;

	/* check if the user entered valid port */
	if( (PORT != PORTA)  && (PORT != PORTB) && (PORT != PORTC) )
	{
		LocalError = GPIO_ERR_OK;
	}
	else
	{
		switch(Value)
		{
			case HIGH:
						PortRegisterPtr->GPIO_BSRR = P_NUM;
					break;

			case LOW:
						PortRegisterPtr->GPIO_BRR = P_NUM;
					break;
			default:
						LocalError = GPIO_ERR_NOK;
					break;
		}
	}

	return LocalError;
}


GPIO_ErrorStatus_t GPIO_writePortValue(void* PORT , uint16_t PortValue)
{
	volatile PortRegister_t* PortRegisterPtr;
	PortRegisterPtr = (PortRegister_t*) PORT;

	/* creating local Error to return the status of the function after finishing */
	GPIO_ErrorStatus_t LocalError = GPIO_ERR_OK;

	/* check if the user entered valid port */
	if( (PORT != PORTA)  && (PORT != PORTB) && (PORT != PORTC) )
	{
		LocalError = GPIO_ERR_NOK;
	}
	else
	{
		PortRegisterPtr->GPIO_ODR = PortValue;
	}

	return LocalError;
}


GPIO_ErrorStatus_t GPIO_ReadPinValue(void* PORT , uint16_t P_NUM , uint8_t* Value)
{
	volatile PortRegister_t* PortRegisterPtr;
	PortRegisterPtr = (PortRegister_t*) PORT;

	/* creating local Error to return the status of the function after finishing */
	GPIO_ErrorStatus_t LocalError = GPIO_ERR_OK;

	/* check if the user entered valid port */
	if( (PORT != PORTA)  && (PORT != PORTB) && (PORT != PORTC) )
	{
		LocalError = GPIO_ERR_NOK;
	}
	else
	{
		if( (PortRegisterPtr->GPIO_IDR & P_NUM) == 0)
		{
			*Value = LOW;
		}
		else
		{
			*Value = HIGH;
		}
	}

	return LocalError;
}
