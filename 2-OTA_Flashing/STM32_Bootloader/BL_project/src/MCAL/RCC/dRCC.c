/***************************************************************************************************/
/* 											RCC Driver												*/
/*                                                                                                  */
/*  Created on	: Jan 14, 2020                                                                      */
/*  Author		: FatmaGomaa  																		*/
/*  Versions	:																					*/
/*  V1 -> in this version:																			*/
/*  - ControlClock 	API let the user choose the clock source and its state [ON-OFF	]               */
/*  - Set System Clock API selects the System Clock by taking its name                              */
/*	- get System Clock API returns the System Clock by taking its name                              */
/*  - ControlPerihperal enables the clock on the selected peripheral                                */
/*  - ConfigPLL takes the System clock for Pll and selects the Mult Value                           */
/*                                                                                                  */
/****************************************************************************************************/
#include "STD_TYPES.h"
#include "dRCC.h"

#define RCC_BASE_ADDRESS		0x40021000
#define	RCC_CR					*((volatile u32*)(RCC_BASE_ADDRESS + 0X00))
#define	RCC_CFGR				*((volatile u32*)(RCC_BASE_ADDRESS + 0X04))
#define	RCC_CIR					*((volatile u32*)(RCC_BASE_ADDRESS + 0X08))
#define	RCC_APB2RSTR			*((volatile u32*)(RCC_BASE_ADDRESS + 0X0C))
#define	RCC_APB1RSTR			*((volatile u32*)(RCC_BASE_ADDRESS + 0X10))
#define	RCC_AHBENR				*((volatile u32*)(RCC_BASE_ADDRESS + 0X14))
#define	RCC_APB2ENR				*((volatile u32*)(RCC_BASE_ADDRESS + 0X18))
#define	RCC_APB1ENR				*((volatile u32*)(RCC_BASE_ADDRESS + 0X1C))
#define	RCC_BDCR				*((volatile u32*)(RCC_BASE_ADDRESS + 0X20))
#define	RCC_CSR					*((volatile u32*)(RCC_BASE_ADDRESS + 0X24))

/*these values will be each clock type MASK*/
#define RCC_CR_HSI_ON_MASK					0x00000001
#define RCC_CR_HSI_RDY_MASK					0x00000002
#define RCC_CR_HSITRIM_MASK					0x000000F8
#define RCC_CR_HSE_ON_MASK					0x00010000
#define RCC_CR_HSE_RDY_MASK					0x00020000
#define RCC_CR_HSE_BYP_MASK					0x00040000
#define RCC_CR_CSS_ON_MASK					0x00080000
#define RCC_CR_PLL_ON_MASK					0x01000000
#define RCC_CR_PLL_RDY_MASK					0x02000000

#define RCC_CFGR_SW_MASK		    		0X00000003
#define RCC_CFGR_SW_CLEAR_MASK		    	0XFFFFFFFC
#define RCC_CFGR_SWS_MASK		    		0X0000000C
#define RCC_CFGR_HPRE_MASK		    		0X000000F0
#define RCC_CFGR_HPRE_CLEAR_MASK		    0XFFFFFF0F
#define RCC_CFGR_PPRE1_MASK		    		0X00000700
#define RCC_CFGR_PPRE1_CLEAR_MASK		    0XFFFFF8FF
#define RCC_CFGR_PPRE2_MASK		    		0X00003800
#define RCC_CFGR_PPRE2_CLEAR_MASK		    0XFFFFC7FF
#define RCC_CFGR_ADCPRE_MASK		    	0X0000C000
#define RCC_CFGR_ADCPRE_CLEAR_MASK		    0XFFFF3FFF
#define RCC_CFGR_PLL_SRC_MASK				0x00010000
#define RCC_CFGR_PLL_XTPRE_MASK				0x00020000
#define RCC_CFGR_PLL_MUL_MASK				0x003C0000
#define RCC_CFGR_PLL_MUL_CLEAR_MASK			0xFFC3FFFF
#define RCC_CFGR_PLL_MCO_MASK				0x07000000
#define RCC_CFGR_PLL_MCO_CLEAR_MASK			0xF8FFFFFF
#define RCC_CSR_SFTRSTF_MASK                0x10000000
#define RCC_CSR_RMVF_MASK                   0x01000000

/*RCC_stdErrorControlClock enables or disables one of the three clocks based on the user input, which is one of the following:
 * First argument: is an object of the CLOCK_TYPE enum with one of the following values:  [HSI_ON,HSE_ON,PLL_ON]
 * second argument is an object of the CLOCK_STATUS enum with one of the following values: [ON,OFF]
 * returns status of the operation whether it's ok or not*/
STD_ERROR RCC_ControlClock(u8 copy_u8ClockType, u8 copy_u8ClockStatus){
	STD_ERROR Local_ControlClockReturnStatus = OK;
	switch(copy_u8ClockType){
	case HSI_ON:
			if(copy_u8ClockStatus == ON){
				RCC_CR |= RCC_CR_HSI_ON_MASK;
				while(!(RCC_CR & RCC_CR_HSI_RDY_MASK));
					return Local_ControlClockReturnStatus;

			}else if(copy_u8ClockStatus == OFF) {
				RCC_CR &=~ RCC_CR_HSI_ON_MASK;
				while((RCC_CR & RCC_CR_HSI_RDY_MASK));
					return Local_ControlClockReturnStatus;
			}
		break;
	case HSE_ON:
		if(copy_u8ClockStatus == ON){
			RCC_CR |= RCC_CR_HSE_ON_MASK;
			while( !(RCC_CR & RCC_CR_HSE_RDY_MASK) );
			return Local_ControlClockReturnStatus;
		}else{
			RCC_CR &=~ RCC_CR_HSE_ON_MASK;
			while( (RCC_CR & RCC_CR_HSE_RDY_MASK) );
			return Local_ControlClockReturnStatus;
		}
		break;
	case PLL_ON:
		if(copy_u8ClockStatus == ON){
			RCC_CR |= RCC_CR_PLL_ON_MASK;
			while( !(RCC_CR & RCC_CR_PLL_RDY_MASK) );
			return Local_ControlClockReturnStatus;
		}else{
			RCC_CR &=~ RCC_CR_PLL_ON_MASK;
			while( (RCC_CR & RCC_CR_PLL_RDY_MASK) );
			return Local_ControlClockReturnStatus;
		}
		break;

	}
	Local_ControlClockReturnStatus =  NOT_OK;
	return Local_ControlClockReturnStatus;
}


/*RCC_stdErrorSetSYSClock selects the system clock
 * {for HSI -> HSI_SW,  for HSE -> HSE_SW,  for PLL -> PLL_SW}
 * */
STD_ERROR RCC_SetSYSClock(u8 copy_u8SystemClock){
	STD_ERROR Local_ControlClockReturnStatus = OK;
	u8 local_temp = RCC_CFGR;
	local_temp &=RCC_CFGR_SW_CLEAR_MASK;
	local_temp |= copy_u8SystemClock;
	RCC_CFGR = local_temp;
	if( (RCC_CFGR & RCC_CFGR_SW_MASK)  ==  copy_u8SystemClock){
		return Local_ControlClockReturnStatus;
	}
	Local_ControlClockReturnStatus = NOT_OK;
	return Local_ControlClockReturnStatus;
}

/*RCC_stdErrorGetSYSClock takes a pointer to variable that will store the value of selected system clock*/
STD_ERROR RCC_GetSYSClock(u32 *copy_u8SystemClock){
	STD_ERROR Local_ControlClockReturnStatus = OK;
	*copy_u8SystemClock = (u8)(RCC_CFGR & RCC_CFGR_SWS_MASK);
	if(*copy_u8SystemClock == (RCC_CFGR & RCC_CFGR_SWS_MASK) ){
		return Local_ControlClockReturnStatus;
	}
	Local_ControlClockReturnStatus = NOT_OK;
	return Local_ControlClockReturnStatus;
}

STD_ERROR RCC_GetPLLMUL(u32 *copy_u8SystemClock){
	STD_ERROR Local_ControlClockReturnStatus = OK;
	*copy_u8SystemClock = (RCC_CFGR & RCC_CFGR_PLL_MUL_MASK);
	if(*copy_u8SystemClock == (RCC_CFGR & RCC_CFGR_PLL_MUL_MASK) ){
		return Local_ControlClockReturnStatus;
	}
	Local_ControlClockReturnStatus = NOT_OK;
	return Local_ControlClockReturnStatus;
}

STD_ERROR RCC_GetPLLSRC(u32 *copy_u8SystemClock){
	STD_ERROR Local_ControlClockReturnStatus = OK;
	*copy_u8SystemClock = (RCC_CFGR & RCC_CFGR_PLL_SRC_MASK);
	if(*copy_u8SystemClock == (RCC_CFGR & RCC_CFGR_PLL_SRC_MASK) ){
		return Local_ControlClockReturnStatus;
	}
	Local_ControlClockReturnStatus = NOT_OK;
	return Local_ControlClockReturnStatus;
}

STD_ERROR RCC_GetPLLXTPRE(u32 *copy_u8SystemClock){
	STD_ERROR Local_ControlClockReturnStatus = OK;
	*copy_u8SystemClock = (RCC_CFGR & RCC_CFGR_PLL_XTPRE_MASK);
	if(*copy_u8SystemClock == (RCC_CFGR & RCC_CFGR_PLL_XTPRE_MASK) ){
		return Local_ControlClockReturnStatus;
	}
	Local_ControlClockReturnStatus = NOT_OK;
	return Local_ControlClockReturnStatus;
}

STD_ERROR RCC_GetHPRE(u32 *copy_u8SystemClock){
	STD_ERROR Local_ControlClockReturnStatus = OK;
	*copy_u8SystemClock = (RCC_CFGR & RCC_CFGR_HPRE_MASK);
	if(*copy_u8SystemClock == (RCC_CFGR & RCC_CFGR_HPRE_MASK) ){
		return Local_ControlClockReturnStatus;
	}
	Local_ControlClockReturnStatus = NOT_OK;
	return Local_ControlClockReturnStatus;
}

STD_ERROR RCC_GetPPRE1(u32 *copy_u8SystemClock){
	STD_ERROR Local_ControlClockReturnStatus = OK;
	*copy_u8SystemClock = (RCC_CFGR & RCC_CFGR_PPRE1_MASK);
	if(*copy_u8SystemClock == (RCC_CFGR & RCC_CFGR_PPRE1_MASK) ){
		return Local_ControlClockReturnStatus;
	}
	Local_ControlClockReturnStatus = NOT_OK;
	return Local_ControlClockReturnStatus;
}
STD_ERROR RCC_GetPPRE2(u32 *copy_u8SystemClock){
	STD_ERROR Local_ControlClockReturnStatus = OK;
	*copy_u8SystemClock = (RCC_CFGR & RCC_CFGR_PPRE2_MASK);
	if(*copy_u8SystemClock == (RCC_CFGR & RCC_CFGR_PPRE2_MASK) ){
		return Local_ControlClockReturnStatus;
	}
	Local_ControlClockReturnStatus = NOT_OK;
	return Local_ControlClockReturnStatus;
}
/*RCC_stdErrorConfigPLL takes one of the following values for
 * first argument:
 * RCC_stdErrorSetPLLSRC chooses the clock for PLL entry. The inputs are:
 * PLLSRC_HSI_DIVIDED_BY_2
 * PLLSRC_HSE
 * PLLSRC_HSE_DIVIDED_BY_2
 *
 * second argument:
 *PLL_INPUT_CLOCK_X_2, PLL_INPUT_CLOCK_X_3, PLL_INPUT_CLOCK_X_4,PLL_INPUT_CLOCK_X_5, PLL_INPUT_CLOCK_X_6
PLL_INPUT_CLOCK_X_7, PLL_INPUT_CLOCK_X_8, PLL_INPUT_CLOCK_X_9, PLL_INPUT_CLOCK_X_10, PLL_INPUT_CLOCK_X_11
PLL_INPUT_CLOCK_X_12, PLL_INPUT_CLOCK_X_13, PLL_INPUT_CLOCK_X_14, PLL_INPUT_CLOCK_X_15, PLL_INPUT_CLOCK_X_16
PLL_INPUT_CLOCK_X_16 */

STD_ERROR RCC_ConfigPLL(u8 copy_u8PLLClockSource, u32 copy_u8PLLMultiplicationFactor){
	STD_ERROR Local_ControlClockReturnStatus = OK;
	u32 local_temp;

	switch(copy_u8PLLClockSource){
	case PLLSRC_HSI_DIVIDED_BY_2:
		RCC_CFGR &=~ RCC_CFGR_PLL_SRC_MASK;
		local_temp = RCC_CFGR;
		local_temp &= RCC_CFGR_PLL_MUL_CLEAR_MASK;
		local_temp |= (copy_u8PLLMultiplicationFactor);
		RCC_CFGR = local_temp;
		//if((RCC_CFGR & RCC_CFGR_PLL_MUL_MASK) == (copy_u8PLLMultiplicationFactor)){

			return Local_ControlClockReturnStatus;
		//}

		break;
	case PLLSRC_HSE:
		RCC_CFGR |= RCC_CFGR_PLL_SRC_MASK;
		local_temp = RCC_CFGR;
		local_temp &=RCC_CFGR_PLL_MUL_CLEAR_MASK;
		local_temp |= copy_u8PLLMultiplicationFactor;
		RCC_CFGR = local_temp;
		if((RCC_CFGR & RCC_CFGR_PLL_MUL_MASK) == copy_u8PLLMultiplicationFactor){
			return Local_ControlClockReturnStatus;
		}
		break;
	case PLLSRC_HSE_DIVIDED_BY_2:
		RCC_CFGR |= RCC_CFGR_PLL_XTPRE_MASK;
		RCC_CFGR |= RCC_CFGR_PLL_SRC_MASK;
		local_temp = RCC_CFGR;
		local_temp &=RCC_CFGR_PLL_MUL_CLEAR_MASK;
		local_temp |= copy_u8PLLMultiplicationFactor;
		RCC_CFGR = local_temp;
		if((RCC_CFGR & RCC_CFGR_PLL_MUL_MASK) == copy_u8PLLMultiplicationFactor){
			return Local_ControlClockReturnStatus;
		}
		break;

	}
	Local_ControlClockReturnStatus = NOT_OK;
	return Local_ControlClockReturnStatus;
}

STD_ERROR RCC_ControlPerihperal(u8 copy_u8Bus, u32 copy_u32Peripheral, u8 status){
	STD_ERROR Local_ErrorStatus = OK;
	switch(copy_u8Bus){
	case AHP:
		if(status == ON){
			RCC_AHBENR |= copy_u32Peripheral;
		}else if(status == OFF){
			RCC_AHBENR &=~ copy_u32Peripheral;
		}
	break;
	case APB1:
		if(status == ON){
			RCC_APB1ENR |= copy_u32Peripheral;
		}else if(status == OFF){
			RCC_APB1ENR &=~ copy_u32Peripheral;
		}
	break;
	case APB2:
		if(status == ON){
		RCC_APB2ENR |= copy_u32Peripheral;
		}else if(status == OFF){
		RCC_APB2ENR &=~ copy_u32Peripheral;
		}
	break;
	}

	return Local_ErrorStatus;

}
/* Check on soft reset */
u8 RCC_GetSFTRSTF(void)
{

	if( RCC_CSR &= RCC_CSR_SFTRSTF_MASK)
	{
		RCC_CSR |=  RCC_CSR_RMVF_MASK;
		return HIGH;
	}
	return LOW;
}

