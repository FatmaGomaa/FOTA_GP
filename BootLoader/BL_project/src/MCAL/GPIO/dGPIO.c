/***************************************************************************************************/
/* 											GPIO Driver												*/
/*                                                                                                  */
/*  Created on	: Feb 4, 2020                                                                       */
/*  Author		: FatmaGomaa  																		*/
/*  Versions	:																					*/
/*  V1 -> in this version:																			*/
/*  - the GPIO Config API receives a structure that holds the information of the 					*/
/*    passed pin                                                                                    */
/*  - GPIO write, writes both High and Low Values on Pin. If it's output, it writes to ODR. If it's */
/*    input it writes to ODR to turn the pull up or pull down register.                        		*/
/*	- GPIO getPinValue returns the reading of a single bit.                                         */
/*                                                                                                  */
/* V1.1 Solved a bug in GPIO_STD_ERROR_Config,the function was writing over the past values which is*/
/* not what we seek, every write cycle shouldn't affect the values of the other pins.   			*/
/* 																									*/
/* V1.2 added GPIO_STD_ERROR_writePort to set the Whole Port Value once.				            */
/*																									*/
/* V1.3 *GPIO_STD_ERROR_Config takes multiple pins (ORED) together                                  */
/* 	   	*GPIO_writePin takes the PORT,PIN,OUTPUT as arguments                             */
/*		*GPIO_u8getPinValue takes the PORT,PIN as arguments                                         */
/****************************************************************************************************/

#include "STD_TYPES.h"
#include "dGPIO.h"


STD_ERROR 	GPIO_Config(GPIO_t * GPIO_Config){
	u8 Local_u8Iteration=0;
	STD_ERROR Local_ErrorStatus = NOT_OK;
	PORT_REGISTERS* GPIO_Config_Struct = (PORT_REGISTERS*) GPIO_Config->PORT;
	u32 temp_GPIO_CRL = LOW;
	u32 temp_GPIO_CRH = LOW;
	if(GPIO_Config->PIN <= 0x00FF){
		temp_GPIO_CRL = GPIO_Config_Struct->GPIO_CRL;
		for(Local_u8Iteration=0;Local_u8Iteration<8;Local_u8Iteration++){
			if(( (GPIO_Config->PIN >> Local_u8Iteration) & 0x0001) == 1 ){
				temp_GPIO_CRL &=~ ((0b1111) << (4*Local_u8Iteration));
				temp_GPIO_CRL |= GPIO_Config->MOOD << (4*Local_u8Iteration);
			}
		}
		GPIO_Config_Struct->GPIO_CRL = temp_GPIO_CRL;
		if(GPIO_Config_Struct->GPIO_CRL == temp_GPIO_CRL){
			Local_ErrorStatus= OK;
		}

	}else{
		temp_GPIO_CRL = GPIO_Config_Struct->GPIO_CRL;
		for(Local_u8Iteration=0;Local_u8Iteration<8;Local_u8Iteration++){
			if(( (GPIO_Config->PIN >> Local_u8Iteration) & 0x0001) == 1 ){
				temp_GPIO_CRL &=~ ((0b1111) << (4*Local_u8Iteration));
				temp_GPIO_CRL |= GPIO_Config->MOOD << (4*Local_u8Iteration);
			}
		}
		GPIO_Config_Struct->GPIO_CRL = temp_GPIO_CRL;

		temp_GPIO_CRH = GPIO_Config_Struct->GIPO_CRH;
		for(Local_u8Iteration=0;Local_u8Iteration<8;Local_u8Iteration++){
			if(( (GPIO_Config->PIN >> (Local_u8Iteration+8)) & 0x0001) == 1 ){
				temp_GPIO_CRH &=~ ((0b1111) << (4*Local_u8Iteration));
				temp_GPIO_CRH |= GPIO_Config->MOOD << (4*Local_u8Iteration);
			}
		}
		GPIO_Config_Struct->GIPO_CRH = temp_GPIO_CRH;
		if((GPIO_Config_Struct->GIPO_CRH == temp_GPIO_CRH) && (GPIO_Config_Struct->GPIO_CRL == temp_GPIO_CRL) ){
			Local_ErrorStatus= OK;
		}
	}
	return Local_ErrorStatus;
}



STD_ERROR GPIO_writePin(volatile void* PORT, u32 PIN, u8 OutputValue){
	STD_ERROR Local_ErrorStatus = NOT_OK;
	PORT_REGISTERS* GPIO_Config_Struct = (PORT_REGISTERS*)PORT;
	if(OutputValue == HIGH){
		GPIO_Config_Struct->GPIO_BSRR = PIN;
	}else if(OutputValue == LOW){
		GPIO_Config_Struct->GIPO_BRR = PIN;
	}
	if(GPIO_Config_Struct->GIPO_ODR == ( GPIO_Config_Struct->GIPO_ODR & PIN )){
		Local_ErrorStatus=OK;
	}
	Local_ErrorStatus=OK;
	return Local_ErrorStatus;

}

STD_ERROR GPIO_STD_ERROR_writePort(volatile void * PORT, u8 OutputValue){
	STD_ERROR Local_ErrorStatus = NOT_OK;
	PORT_REGISTERS * PORT_x = (PORT_REGISTERS *) PORT;
	PORT_x->GIPO_ODR = OutputValue;
	if(PORT_x->GIPO_ODR == OutputValue){
		Local_ErrorStatus=OK;
	}
	return Local_ErrorStatus;

}


u8 GPIO_u8getPinValue(volatile void* PORT, u32 PIN){
	u8 Local_ReturnValue=HIGH;
	PORT_REGISTERS* GPIO_Config_Struct = (PORT_REGISTERS*) PORT;
	//trace_printf("Pin Value %d \n", (GPIO_Config_Struct->GPIO_IDR >> GPIO_Config->PIN)&1);
	Local_ReturnValue=((GPIO_Config_Struct->GPIO_IDR & PIN) >0) ? HIGH:LOW;
	return Local_ReturnValue;
}
