/*
 * hRCC.c
 *
 *  Created on: Apr 5, 2020
 *      Author: FatmaGomaa
 */
#include "STD_TYPES.h"
#include "dRCC.h"
#include "hRCC.h"

#define RCC_HSI_SYSCLK                0x00000000
#define RCC_HSE_SYSCLK                0x00000004
#define RCC_PLL_SYSCLK                0x00000008

#define RCC_SYSCLK_PLL_HSI_DIVIDE_2   0x00000000
#define RCC_SYSCLK_PLL_HSE            0x00010000

#define RCC_SYSCLK_HSE_NOT_DIVID      0X00000000
#define RCC_SYSCLK_HSE_DIVID_2        0X00020000

#define RCC_HSI_VALUE         8

#define RCC_SYSCLK_8_MEGA     8
#define RCC_SYSCLK_4_MEGA     4

#define	AHP_SHIFT				4
#define	AHP_MAPPING_BASE		8
#define	AHP_SUBTRACTED_VALUE	7

#define	APB1_SHIFT				8
#define	APB1_MAPPING_BASE		4
#define	APB1_SUBTRACTED_VALUE	3

#define	APB2_SHIFT				11
#define	APB2_MAPPING_BASE		4
#define	APB2_SUBTRACTED_VALUE	3


void RCC_GetSystemClockMGZ(u8 * systemClock)
{
	u32 MullFactor;
	u32 Clock;

	RCC_GetSYSClock(&Clock);

	/*system clock is HSI*/
	if( Clock == RCC_HSI_SYSCLK)
	{
		* systemClock = RCC_SYSCLK_8_MEGA ;
	}

	/*system clock is HSE*/
	else if(Clock == RCC_HSE_SYSCLK)
	{
		* systemClock = RCC_SYSCLK_8_MEGA ;
	}

	/*system clock source is PLL*/
	else if(Clock == RCC_PLL_SYSCLK)
	{
		/*get the PLLMUL value*/
		RCC_GetPLLMUL(&Clock);
		MullFactor = (Clock >> 18) + 2 ;


		RCC_GetPLLSRC(&Clock);

		/* system clock is HSI/2 * Multiplication Factor */
		if( Clock == RCC_SYSCLK_PLL_HSI_DIVIDE_2)
			* systemClock = (RCC_SYSCLK_4_MEGA * MullFactor);


		else if( Clock == RCC_SYSCLK_PLL_HSE)
		{
			RCC_GetPLLXTPRE(&Clock);

			/*system clock is HSE  * Multiplication Factor*/
			if( Clock == RCC_SYSCLK_HSE_NOT_DIVID)
				* systemClock = (RCC_SYSCLK_8_MEGA * MullFactor);

			/*system clock is HSE/2  * Multiplication Factor*/
			else if(Clock == RCC_SYSCLK_HSE_DIVID_2)
				*systemClock = (RCC_SYSCLK_4_MEGA * MullFactor);

		}

	}
}
 /*	AHP,
	APB1,
	APB2*/
void RCC_GetPeripheralClockMGZ(u8 peripheral, u8 * peripheralClock){
	u8 SystemClock;
	u32 AHP_DivisionFactor =1;
	u32 APB1_DivisionFactor =1;
	u32 APB2_DivisionFactor =1;

	u32 Power;
	u8 iterator;

	RCC_GetSystemClockMGZ(&SystemClock);

	/*get AHP_DivisionFactor*/
	RCC_GetHPRE(&Power);
	Power = (Power >> AHP_SHIFT);
	if(Power >= AHP_MAPPING_BASE){
		Power -= AHP_SUBTRACTED_VALUE;
		for (iterator =0 ; iterator < Power ; iterator++){
			AHP_DivisionFactor *= iterator;
		}
	}

	switch (peripheral) {
	case AHP:
		*peripheralClock = (SystemClock / AHP_DivisionFactor);
		break;
	case APB1:
		/*get APB1_DivisionFactor*/
		RCC_GetPPRE1(&Power);
		Power = (Power >> APB1_SHIFT) ;
		if(Power >= APB1_MAPPING_BASE){
			Power -= APB1_SUBTRACTED_VALUE;
			for (iterator =0 ; iterator < Power ; iterator++){
				APB1_DivisionFactor *= iterator;
			}
		}
		*peripheralClock =  (SystemClock / (APB1_DivisionFactor * AHP_DivisionFactor));

		break;
	case APB2:

		/*get APB2_DivisionFactor*/
		RCC_GetPPRE2(&Power);
		Power = (Power >> APB2_SHIFT);
		if(Power >= APB2_MAPPING_BASE){
			Power -= APB2_SUBTRACTED_VALUE;
			for (iterator =0 ; iterator < Power ; iterator++){
				APB2_DivisionFactor *= iterator;
			}
		}
		*peripheralClock =  (SystemClock / (APB2_DivisionFactor * AHP_DivisionFactor));

		break;
	}

}
