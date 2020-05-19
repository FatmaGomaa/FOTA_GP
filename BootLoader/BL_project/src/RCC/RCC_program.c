/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * File name: RCC_program.h                                                                            *
 *                                                                                                     *
 * Description: This source file contains the implementations of APIs of the  RCC module in stm32f10x  *
 *  evaluation kit  																				   *
 *                                                                                                     *
 * Author : Mohamed Adel Anis Ahmed				                                                       *
 * date   : 13 May 2020                                                                                *
 * version: 2.0                                                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/*====================================================================================================*/
/***********************************************Includes***********************************************/
/*====================================================================================================*/
#include "STD_TYPES.h"

#include "RCC_interface.h"

/*====================================================================================================*/
/************************************************Macros************************************************/
/*====================================================================================================*/
#define RCC_BASE_ADDRESS			((volatile RCC_Register_t *)(0x40021000))

#define CR_HSI_RDY_MASK				(0x00000002)
#define CR_HSE_RDY_MASK				(0x00020000)
#define CR_PLL_RDY_MASK				(0x02000000)
#define CR_HSE_BYBASS_MASK			(0x00040000)
#define CR_HSE_BYPASS_Clear			(0xFFFBFFFF)
#define CR_CSS_ON_MASK				(0x00080000)
#define CR_CSS_OFF_MASK				(0xFFF7FFFF)

#define CFGR_SW_CLEAR				(0xFFFFFFFC)
#define CFGR_SWS_MASK				(0x0000000C)
#define CFGR_SWS_ALIGN_FACTOR		(2)
#define CFGR_PLL_SRC_CLK_CLEAR		(0xFFFCFFFF)
#define CFGR_PLL_CLK_MULTI_CLEAR	(0xFFC3FFFF)
#define CFGR_MCO_CLEAR				(0xF8FFFFFF)
#define CFGR_ADC_PCLK2_CLEAR		(0xFFFF3FFF)
#define CFGR_APB2_HCLK_CLEAR		(0xFFFFC7FF)
#define CFGR_APB1_HCLK_CLEAR		(0xFFFFF8FF)

/*====================================================================================================*/
/*********************************************Definitions**********************************************/
/*====================================================================================================*/

typedef struct{

	uint32_t RCC_CR;
	uint32_t RCC_CFGR;
	uint32_t RCC_CIR;
	uint32_t RCC_APB2RSTR;
	uint32_t RCC_APB1RSTR;
	uint32_t RCC_AHBENR;
	uint32_t RCC_APB2ENR;
	uint32_t RCC_APB1ENR;
	uint32_t RCC_BDCR;
	uint32_t RCC_CSR;

}RCC_Register_t;

/*====================================================================================================*/
/*****************************************APIs Implementation******************************************/
/*====================================================================================================*/



/******************************************************************************************************
 * Description : This Function is used to set the status of specific clock to be ON or OFF.
 *
 */
RCC_ErrorStatus_t RCC_SetClkStatus(uint32_t RCC_CLK , RCC_Status_t RCC_STAT)
{
	volatile RCC_Register_t *RCC_Register = RCC_BASE_ADDRESS;
	RCC_ErrorStatus_t Local_Error = RCC_ERR_OK;

	/* Check the Argument */
	if( (RCC_CLK != RCC_CLK_HSI)  && (RCC_CLK != RCC_CLK_HSE) && (RCC_CLK != RCC_CLK_PLL) )
	{
		Local_Error = RCC_ERR_WRONG_ARG;
	}
	else
	{
		/* Change the status of the desired clk */
		switch(RCC_STAT)
		{
		case RCC_STAT_ON:
			RCC_Register->RCC_CR |= RCC_CLK;
			break;

		case RCC_STAT_OFF:
			RCC_Register->RCC_CR &= (~RCC_CLK);
			break;

		/* Wrong Argument */
		default:
			Local_Error = RCC_ERR_WRONG_ARG;
		}


		/* check if the clock has been set right */
		switch(RCC_STAT)
		{
		case RCC_STAT_ON:
			if( (RCC_Register->RCC_CR & (RCC_CLK << SHIFT_1_BIT)) == 0)
			{
				Local_Error = RCC_ERR_FAILED_CONFIG;
			}
			else
			{
				/* Do Nothing */
			}
			break;

		case RCC_STAT_OFF:
			if( (RCC_Register->RCC_CR & (RCC_CLK << SHIFT_1_BIT)) != 0)
			{
				Local_Error = RCC_ERR_FAILED_CONFIG;
			}
			else
			{
				/* Do Nothing */
			}
			break;

		}
	}


	return Local_Error;
}


/******************************************************************************************************
 * Description : This Function is used to Select the desired Clk to be Selected as System clk
 *
 */
RCC_ErrorStatus_t RCC_SelectSystemClk(uint8_t RCC_SYS_CLK)
{
	volatile RCC_Register_t *RCC_Register = RCC_BASE_ADDRESS;
	RCC_ErrorStatus_t Local_Error = RCC_ERR_OK;

	if( (RCC_SYS_CLK != RCC_SYS_CLK_HSI)  && (RCC_SYS_CLK != RCC_SYS_CLK_HSE) && (RCC_SYS_CLK != RCC_SYS_CLK_PLL) )
	{
		Local_Error = RCC_ERR_WRONG_ARG;
	}
	else
	{
		/* Clear the Value without changing the Register itself then apply the select the new clk */
		RCC_Register->RCC_CFGR = (RCC_Register->RCC_CFGR & CFGR_SW_CLEAR) | RCC_SYS_CLK;

		/* Check that the desired clk has been selected correctly */
		if( (RCC_Register->RCC_CFGR & CFGR_SWS_MASK) != (uint32_t)(RCC_SYS_CLK << CFGR_SWS_ALIGN_FACTOR) )
		{
			Local_Error = RCC_ERR_FAILED_CONFIG;
		}
		else
		{
			/* Do Nothing */
		}
	}


	return Local_Error;
}


/******************************************************************************************************
 * Description       : This Function is used to enable or disable the bypass mode of HSE clock.
 *
 * Needed Conditions : The HSE clock needs to be closed while configuring The BYPASS option.
 *
 */
RCC_ErrorStatus_t RCC_HSE_ConfigBypass(uint8_t RCC_BYPASS)
{
	volatile RCC_Register_t *RCC_Register = RCC_BASE_ADDRESS;
	RCC_ErrorStatus_t Local_Error = RCC_ERR_OK;

	if( (RCC_BYPASS != RCC_BYPASS_ON) && (RCC_BYPASS != RCC_BYPASS_OFF)  )
	{
		Local_Error = RCC_ERR_WRONG_ARG;
	}
	else
	{
		/* Check if the HSE is ON OR OFF (if ON return Error) */
		if( (RCC_Register->RCC_CR & CR_HSE_RDY_MASK) == 0)
		{
			switch(RCC_BYPASS)
			{
			case RCC_BYPASS_ON:

				RCC_Register->RCC_CR |= CR_HSE_BYBASS_MASK;

				break;

			case RCC_BYPASS_OFF:

				RCC_Register->RCC_CR &= CR_HSE_BYPASS_Clear;

				break;
			}
		}
		else
		{
			Local_Error = RCC_ERR_RUNNING_UNCONFIGURAABLE;
		}
	}



	return Local_Error;
}


/******************************************************************************************************
 * Description       : This Function is used to configure the PLL clock's source and its value.
 *
 * Needed Conditions : The PLL clock needs to be closed while configuring The Clock options
 *
 */
RCC_ErrorStatus_t RCC_PLL_Config(uint32_t RCC_PLL_Source_Clk , uint32_t RCC_PLL_Clk_Multi )
{
	volatile RCC_Register_t *RCC_Register = RCC_BASE_ADDRESS;
	RCC_ErrorStatus_t Local_Error = RCC_ERR_OK;

	/* Check if the PLL is ON OR OFF (if ON return Error) */
		if( (RCC_Register->RCC_CR & CR_PLL_RDY_MASK) == 0)
		{

			RCC_Register->RCC_CFGR = (RCC_Register->RCC_CFGR & CFGR_PLL_SRC_CLK_CLEAR) | RCC_PLL_Source_Clk;

			RCC_Register->RCC_CFGR = (RCC_Register->RCC_CFGR & CFGR_PLL_CLK_MULTI_CLEAR)  | RCC_PLL_Clk_Multi;
		}
		else
		{
			Local_Error = RCC_ERR_RUNNING_UNCONFIGURAABLE;
		}

	return Local_Error;
}



/******************************************************************************************************
 * Description       : This Function is used to enable or disable the Clock Security system.
 *
 * Needed Conditions : The HSE clock needs to be Enabled while configuring The CSS option.
 *
 */
RCC_ErrorStatus_t RCC_CSS_Config(uint32_t RCC_CSS)
{
	volatile RCC_Register_t *RCC_Register = RCC_BASE_ADDRESS;
	RCC_ErrorStatus_t Local_Error = RCC_ERR_OK;

	if( (RCC_CSS != RCC_CSS_ON) && (RCC_CSS != RCC_CSS_OFF) )
	{
		Local_Error = RCC_ERR_WRONG_ARG;
	}
	else
	{
		/* Check if the HSE is ON OR OFF (if OFF return Error) */
		if( (RCC_Register->RCC_CR & CR_HSE_RDY_MASK) != 0)
		{
			switch(RCC_CSS)
			{
			case RCC_CSS_ON:
				RCC_Register->RCC_CR |= CR_CSS_ON_MASK ;
				break;
			case RCC_CSS_OFF:
				RCC_Register->RCC_CR &= CR_CSS_OFF_MASK;
				break;

			default:
				Local_Error = RCC_ERR_FAILED_CONFIG;
			}

		}
		else
		{
			Local_Error = RCC_ERR_FAILED_CONFIG;
		}
	}


	return Local_Error;
}



RCC_ErrorStatus_t RCC_MCO_Config(uint32_t RCC_MCO)
{
	volatile RCC_Register_t *RCC_Register = RCC_BASE_ADDRESS;
	RCC_ErrorStatus_t Local_Error = RCC_ERR_OK;

	if( (RCC_MCO != RCC_MCO_NO_CLK) && (RCC_MCO != RCC_MCO_SYS_CLK) && (RCC_MCO != RCC_MCO_HSI_CLK) && (RCC_MCO != RCC_MCO_HSE_CLK) && (RCC_MCO != RCC_MCO_PLL_CLK) )
	{
		Local_Error = RCC_ERR_WRONG_ARG;
	}
	else
	{
		RCC_Register->RCC_CFGR |= (RCC_Register->RCC_CFGR & CFGR_MCO_CLEAR) | RCC_MCO;
	}

	return Local_Error;
}



RCC_ErrorStatus_t RCC_ADC_ConfigPrescaler(uint32_t RCC_ADC_PCLK2)
{
	volatile RCC_Register_t *RCC_Register = RCC_BASE_ADDRESS;
	RCC_ErrorStatus_t Local_Error = RCC_ERR_OK;

	if( (RCC_ADC_PCLK2 != RCC_ADC_PCLK2_2) && (RCC_ADC_PCLK2 != RCC_ADC_PCLK2_4) && (RCC_ADC_PCLK2 != RCC_ADC_PCLK2_6) && (RCC_ADC_PCLK2 != RCC_ADC_PCLK2_8) )
	{
		Local_Error = RCC_ERR_WRONG_ARG;
	}
	else
	{
		RCC_Register->RCC_CFGR |= (RCC_Register->RCC_CFGR & CFGR_ADC_PCLK2_CLEAR) | RCC_ADC_PCLK2;
	}

	return Local_Error;
}




RCC_ErrorStatus_t RCC_APB2_ConfigPrescaler(uint32_t RCC_APB2_HCLK)
{
	volatile RCC_Register_t *RCC_Register = RCC_BASE_ADDRESS;
	RCC_ErrorStatus_t Local_Error = RCC_ERR_OK;

	if( (RCC_APB2_HCLK != RCC_APB2_HCLK_NO_DIV) && (RCC_APB2_HCLK != RCC_APB2_HCLK_DIV_2) && (RCC_APB2_HCLK != RCC_APB2_HCLK_DIV_4) && (RCC_APB2_HCLK != RCC_APB2_HCLK_DIV_8) && (RCC_APB2_HCLK != RCC_APB2_HCLK_DIV_16) )
	{
		Local_Error = RCC_ERR_WRONG_ARG;
	}
	else
	{
		RCC_Register->RCC_CFGR |= (RCC_Register->RCC_CFGR & CFGR_APB2_HCLK_CLEAR) | RCC_APB2_HCLK;
	}


	return Local_Error;
}



RCC_ErrorStatus_t RCC_APB1_ConfigPrescaler(uint32_t RCC_APB1_HCLK)
{
	volatile RCC_Register_t *RCC_Register = RCC_BASE_ADDRESS;
	RCC_ErrorStatus_t Local_Error = RCC_ERR_OK;

	if( (RCC_APB1_HCLK != RCC_APB1_HCLK_NO_DIV) && (RCC_APB1_HCLK != RCC_APB1_HCLK_DIV_2) && (RCC_APB1_HCLK != RCC_APB1_HCLK_DIV_4) && (RCC_APB1_HCLK != RCC_APB1_HCLK_DIV_8) && (RCC_APB1_HCLK != RCC_APB1_HCLK_DIV_16) )
	{
		Local_Error = RCC_ERR_WRONG_ARG;
	}
	else
	{
		RCC_Register->RCC_CFGR |= (RCC_Register->RCC_CFGR & CFGR_APB1_HCLK_CLEAR) | RCC_APB1_HCLK;
	}


	return Local_Error;
}



RCC_ErrorStatus_t RCC_APB2_ResetPeripheral(uint32_t RCC_APB2_RESET)
{
	volatile RCC_Register_t *RCC_Register = RCC_BASE_ADDRESS;
	RCC_ErrorStatus_t Local_Error = RCC_ERR_OK;

	RCC_Register->RCC_APB2RSTR = RCC_APB2_RESET;

	return Local_Error;
}



RCC_ErrorStatus_t RCC_APB1_ResetPeripheral(uint32_t RCC_APB1_RESET)
{
	volatile RCC_Register_t *RCC_Register = RCC_BASE_ADDRESS;
	RCC_ErrorStatus_t Local_Error = RCC_ERR_OK;

	RCC_Register->RCC_APB1RSTR = RCC_APB1_RESET;

	return Local_Error;
}


RCC_ErrorStatus_t RCC_AHB_SetPeripheralClk(uint32_t RCC_AHB_CLK , RCC_Status_t RCC_STAT )
{
	volatile RCC_Register_t *RCC_Register = RCC_BASE_ADDRESS;
	RCC_ErrorStatus_t Local_Error = RCC_ERR_OK;

	switch(RCC_STAT)
	{
	case RCC_STAT_ON:
		RCC_Register->RCC_AHBENR |= RCC_AHB_CLK;
		break;

	case RCC_STAT_OFF:
		RCC_Register->RCC_AHBENR &= (~RCC_AHB_CLK);
		break;

	default:
		Local_Error = RCC_ERR_WRONG_ARG;
	}

	return Local_Error;
}


RCC_ErrorStatus_t RCC_APB2_SetPeripheralClk(uint32_t RCC_APB2_CLK , RCC_Status_t RCC_STAT)
{
	volatile RCC_Register_t *RCC_Register = RCC_BASE_ADDRESS;
	RCC_ErrorStatus_t Local_Error = RCC_ERR_OK;

	switch(RCC_STAT)
	{
	case RCC_STAT_ON:
		RCC_Register->RCC_APB2ENR |= RCC_APB2_CLK;
		break;

	case RCC_STAT_OFF:
		RCC_Register->RCC_APB2ENR &= (~RCC_APB2_CLK);
		break;

	default:
		Local_Error = RCC_ERR_WRONG_ARG;
	}

	return Local_Error;
}



RCC_ErrorStatus_t RCC_APB1_SetPeripheralClk(uint32_t RCC_APB1_CLK , RCC_Status_t RCC_STAT)
{
	volatile RCC_Register_t *RCC_Register = RCC_BASE_ADDRESS;
	RCC_ErrorStatus_t Local_Error = RCC_ERR_OK;

	switch(RCC_STAT)
	{
	case RCC_STAT_ON:
		RCC_Register->RCC_APB1ENR |= RCC_APB1_CLK;
		break;

	case RCC_STAT_OFF:
		RCC_Register->RCC_APB1ENR &= (~RCC_APB1_CLK);
		break;

	default:
		Local_Error = RCC_ERR_WRONG_ARG;
	}

	return Local_Error;
}
