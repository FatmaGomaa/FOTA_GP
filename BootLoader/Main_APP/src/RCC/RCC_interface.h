/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * File name: RCC_interface.h                                                                          *
 *                                                                                                     *
 * Description: This Header file is used to interface with the RCC module in stm32f10x evaluation kit  *
 *                                                                                                     *
 * Author : Mohamed Adel Anis Ahmed				                                                       *
 * date   : 13 May 2020                                                                                *
 * version: 2.0                                                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef RCC_RCC_INTERFACE_H_
#define RCC_RCC_INTERFACE_H_

/*====================================================================================================*/
/****************************************Macros & Definitions******************************************/
/*====================================================================================================*/

/* Types of clocks that can be chosen to be turn on or off */
#define RCC_CLK_HSI							(0x00000001)
#define RCC_CLK_HSE							(0x00010000)
#define RCC_CLK_PLL							(0x01000000)

/* System clock Options */
#define RCC_SYS_CLK_HSI						(0U)
#define	RCC_SYS_CLK_HSE						(1U)
#define RCC_SYS_CLK_PLL						(2U)

/* HSE Configs Options */
#define RCC_BYPASS_ON						(1)
#define RCC_BYPASS_OFF						(0)

/* CSS Configs Options*/
#define RCC_CSS_ON							(1)
#define RCC_CSS_OFF							(0)

/* PLL CLK Source Options */
#define RCC_PLL_Src_CLK_HALF_HSI			(0x00000000)
#define RCC_PLL_Src_CLK_HSE					(0x00010000)
#define RCC_PLL_Src_CLK_HALF_HSE			(0x00030000)

/* PLL CLK Source Multiplication Options */
#define RCC_PLL_CLK_Multi_2					(0x00000000)
#define RCC_PLL_CLK_Multi_3					(0x00040000)
#define RCC_PLL_CLK_Multi_4					(0x00080000)
#define RCC_PLL_CLK_Multi_5					(0x000C0000)
#define RCC_PLL_CLK_Multi_6					(0x00100000)
#define RCC_PLL_CLK_Multi_7					(0x00140000)
#define RCC_PLL_CLK_Multi_8					(0x00180000)
#define RCC_PLL_CLK_Multi_9					(0x001C0000)
#define RCC_PLL_CLK_Multi_10				(0x00200000)
#define RCC_PLL_CLK_Multi_11				(0x00240000)
#define RCC_PLL_CLK_Multi_12				(0x00280000)
#define RCC_PLL_CLK_Multi_13				(0x002C0000)
#define RCC_PLL_CLK_Multi_14				(0x00300000)
#define RCC_PLL_CLK_Multi_15				(0x00340000)
#define RCC_PLL_CLK_Multi_16				(0x00380000)

/* MCO Options */
#define RCC_MCO_NO_CLK						(0x00000000)
#define RCC_MCO_SYS_CLK						(0x04000000)
#define RCC_MCO_HSI_CLK						(0x05000000)
#define RCC_MCO_HSE_CLK						(0x06000000)
#define RCC_MCO_PLL_CLK						(0x07000000)

/* ADC CLK Prescaler */
#define RCC_ADC_PCLK2_2						(0x00000000)
#define RCC_ADC_PCLK2_4						(0x00004000)
#define RCC_ADC_PCLK2_6						(0x00008000)
#define RCC_ADC_PCLK2_8						(0x0000C000)

/* APB2 Bus Clk Prescaler */
#define RCC_APB2_HCLK_NO_DIV				(0x00000000)
#define RCC_APB2_HCLK_DIV_2					(0x00002000)
#define RCC_APB2_HCLK_DIV_4					(0x00002800)
#define RCC_APB2_HCLK_DIV_8					(0x00003000)
#define RCC_APB2_HCLK_DIV_16				(0x00003800)

/* APB2 Bus Clk Prescaler */
#define RCC_APB1_HCLK_NO_DIV				(0x00000000)
#define RCC_APB1_HCLK_DIV_2					(0x00000400)
#define RCC_APB1_HCLK_DIV_4					(0x00000500)
#define RCC_APB1_HCLK_DIV_8					(0x00000600)
#define RCC_APB1_HCLK_DIV_16				(0x00000700)

/* APB2 Peripherals Reset Options */
#define RCC_APB2_RESET_AFIO					(0x00000001)
#define RCC_APB2_RESET_PORTA				(0x00000004)
#define RCC_APB2_RESET_PORTB				(0x00000008)
#define RCC_APB2_RESET_PORTC				(0x00000010)
#define RCC_APB2_RESET_PORTD				(0x00000020)
#define RCC_APB2_RESET_PORTE				(0x00000040)
#define RCC_APB2_RESET_PORTF				(0x00000080)
#define RCC_APB2_RESET_PORTG				(0x00000100)
#define RCC_APB2_RESET_ADC1					(0x00000200)
#define RCC_APB2_RESET_ADC2					(0x00000400)
#define RCC_APB2_RESET_TIMER1				(0x00000800)
#define RCC_APB2_RESET_SPI1					(0x00001000)
#define RCC_APB2_RESET_TIMER8				(0x00002000)
#define RCC_APB2_RESET_USART1				(0x00004000)
#define RCC_APB2_RESET_ADC3					(0x00008000)
#define RCC_APB2_RESET_TIMER9				(0x00080000)
#define RCC_APB2_RESET_TIMER10				(0x00100000)
#define RCC_APB2_RESET_TIMER11				(0x00200000)

/* APB1 Peripheral Reset Options */
#define RCC_APB1_RESET_TIMER2				(0x00000001)
#define RCC_APB1_RESET_TIMER3				(0x00000002)
#define RCC_APB1_RESET_TIMER4				(0x00000004)
#define RCC_APB1_RESET_TIMER5				(0x00000008)
#define RCC_APB1_RESET_TIMER6				(0x00000010)
#define RCC_APB1_RESET_TIMER7				(0x00000020)
#define RCC_APB1_RESET_TIMER12				(0x00000040)
#define RCC_APB1_RESET_TIMER13				(0x00000080)
#define RCC_APB1_RESET_TIMER14				(0x00000100)
#define RCC_APB1_RESET_WWDG					(0x00000800)
#define RCC_APB1_RESET_SPI2					(0x00004000)
#define RCC_APB1_RESET_SPI3					(0x00008000)
#define RCC_APB1_RESET_USART2				(0x00020000)
#define RCC_APB1_RESET_USART3				(0x00040000)
#define RCC_APB1_RESET_USART4				(0x00080000)
#define RCC_APB1_RESET_USART5				(0x00100000)
#define RCC_APB1_RESET_I2C1					(0x00200000)
#define RCC_APB1_RESET_I2C2					(0x00400000)
#define RCC_APB1_RESET_USB					(0x00800000)
#define RCC_APB1_RESET_CAN					(0x02000000)
#define RCC_APB1_RESET_BKP					(0x08000000)
#define RCC_APB1_RESET_PWR					(0x10000000)
#define RCC_APB1_RESET_DAC					(0x20000000)

/* AHB Peripheral CLK Enable Options */
#define RCC_AHB_CLK_DMA1					(0x00000001)
#define RCC_AHB_CLK_DMA2					(0x00000002)
#define RCC_AHB_CLK_SRAM					(0x00000004)
#define RCC_AHB_CLK_FLITF					(0x00000010)
#define RCC_AHB_CLK_CRCE					(0x00000040)
#define RCC_AHB_CLK_FSMC					(0x00000100)
#define RCC_AHB_CLK_SDIO					(0x00000400)

/* APB2 Peripheral Clk Options */
#define RCC_APB2_CLK_AFIO					(0x00000001)
#define RCC_APB2_CLK_PORTA					(0x00000004)
#define RCC_APB2_CLK_PORTB					(0x00000008)
#define RCC_APB2_CLK_PORTC					(0x00000010)
#define RCC_APB2_CLK_PORTD					(0x00000020)
#define RCC_APB2_CLK_PORTE					(0x00000040)
#define RCC_APB2_CLK_PORTF					(0x00000080)
#define RCC_APB2_CLK_PORTG					(0x00000100)
#define RCC_APB2_CLK_ADC1					(0x00000200)
#define RCC_APB2_CLK_ADC2					(0x00000400)
#define RCC_APB2_CLK_TIMER1					(0x00000800)
#define RCC_APB2_CLK_SPI1					(0x00001000)
#define RCC_APB2_CLK_TIMER8					(0x00002000)
#define RCC_APB2_CLK_USART1					(0x00004000)
#define RCC_APB2_CLK_ADC3					(0x00008000)
#define RCC_APB2_CLK_TIMER9					(0x00080000)
#define RCC_APB2_CLK_TIMER10				(0x00100000)
#define RCC_APB2_CLK_TIMER11				(0x00200000)

/* APB1 Peripheral Clk Options */
#define RCC_APB1_CLK_TIMER2					(0x00000001)
#define RCC_APB1_CLK_TIMER3					(0x00000002)
#define RCC_APB1_CLK_TIMER4					(0x00000004)
#define RCC_APB1_CLK_TIMER5					(0x00000008)
#define RCC_APB1_CLK_TIMER6					(0x00000010)
#define RCC_APB1_CLK_TIMER7					(0x00000020)
#define RCC_APB1_CLK_TIMER12				(0x00000040)
#define RCC_APB1_CLK_TIMER13				(0x00000080)
#define RCC_APB1_CLK_TIMER14				(0x00000100)
#define RCC_APB1_CLK_WWDG					(0x00000800)
#define RCC_APB1_CLK_SPI2					(0x00004000)
#define RCC_APB1_CLK_SPI3					(0x00008000)
#define RCC_APB1_CLK_USART2					(0x00020000)
#define RCC_APB1_CLK_USART3					(0x00040000)
#define RCC_APB1_CLK_USART4					(0x00080000)
#define RCC_APB1_CLK_USART5					(0x00100000)
#define RCC_APB1_CLK_I2C1					(0x00200000)
#define RCC_APB1_CLK_I2C2					(0x00400000)
#define RCC_APB1_CLK_USB					(0x00800000)
#define RCC_APB1_CLK_CAN					(0x02000000)
#define RCC_APB1_CLK_BKP					(0x08000000)
#define RCC_APB1_CLK_PWR					(0x10000000)
#define RCC_APB1_CLK_DAC					(0x20000000)

/*====================================================================================================*/
/*********************************************Error Types**********************************************/
/*====================================================================================================*/
typedef uint8_t	RCC_ErrorStatus_t;

#define RCC_ERR_NOK							(0)
#define RCC_ERR_OK							(1)
#define RCC_ERR_RUNNING_UNCONFIGURAABLE		(2)
#define RCC_ERR_WRONG_ARG					(3)
#define RCC_ERR_FAILED_CONFIG				(4)

/*====================================================================================================*/
/*********************************************Miscellaneous********************************************/
/*====================================================================================================*/
typedef uint8_t RCC_Status_t;

#define RCC_STAT_ON							(1)
#define RCC_STAT_OFF						(0)


/*====================================================================================================*/
/*********************************************Prototypes***********************************************/
/*====================================================================================================*/

RCC_ErrorStatus_t RCC_SetClkStatus(uint32_t RCC_CLK , RCC_Status_t RCC_STAT);
RCC_ErrorStatus_t RCC_SelectSystemClk(uint8_t RCC_SYS_CLK);

RCC_ErrorStatus_t RCC_APB2_SetPeripheralClk(uint32_t RCC_APB2_CLK , RCC_Status_t RCC_STAT);
RCC_ErrorStatus_t RCC_APB1_SetPeripheralClk(uint32_t RCC_APB1_CLK , RCC_Status_t RCC_STAT);
RCC_ErrorStatus_t RCC_AHB_SetPeripheralClk(uint32_t RCC_AHB_CLK , RCC_Status_t RCC_STAT);


RCC_ErrorStatus_t RCC_HSE_ConfigBypass(uint8_t RCC_BYPASS);
RCC_ErrorStatus_t RCC_PLL_Config(uint32_t RCC_PLL_Src_CLK , uint32_t RCC_PLL_CLK_Multi );
RCC_ErrorStatus_t RCC_CSS_Config(uint32_t RCC_CSS);
RCC_ErrorStatus_t RCC_MCO_Config(uint32_t RCC_MCO);
RCC_ErrorStatus_t RCC_ADC_ConfigPrescaler(uint32_t RCC_ADC_PCLK2);
RCC_ErrorStatus_t RCC_APB2_ConfigPrescaler(uint32_t RCC_APB2_HCLK);
RCC_ErrorStatus_t RCC_APB1_ConfigPrescaler(uint32_t RCC_APB1_HCLK);

RCC_ErrorStatus_t RCC_APB2_ResetPeripheral(uint32_t RCC_APB2_RESET);
RCC_ErrorStatus_t RCC_APB1_ResetPeripheral(uint32_t RCC_APB1_RESET);

#endif /* RCC_RCC_INTERFACE_H_ */
