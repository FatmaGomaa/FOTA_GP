

#ifndef DRCC_H_
#define DRCC_H_

#include "STD_TYPES.h"

#define HSI_SW           	        0b00
#define HSE_SW			 	        0b01
#define PLL_SW				        0b10

#define SYSCLK_DIVIDED_BY_2         0b1000
#define SYSCLK_DIVIDED_BY_4         0b1001
#define SYSCLK_DIVIDED_BY_8         0b1010
#define SYSCLK_DIVIDED_BY_16        0b1011
#define SYSCLK_DIVIDED_BY_64        0b1100
#define SYSCLK_DIVIDED_BY_128       0b1101
#define SYSCLK_DIVIDED_BY_256       0b1110
#define SYSCLK_DIVIDED_BY_512       0b1111

#define HCLK_DIVIDED_BY_2           0b100
#define HCLK_DIVIDED_BY_4           0b101
#define HCLK_DIVIDED_BY_8           0b110
#define HCLK_DIVIDED_BY_16          0b111

#define HCLK_DIVIDED_BY_2           0b100
#define HCLK_DIVIDED_BY_4           0b101
#define HCLK_DIVIDED_BY_8           0b110
#define HCLK_DIVIDED_BY_16          0b111

#define  PLL_INPUT_CLOCK_X_2      0x00000000
#define  PLL_INPUT_CLOCK_X_3      0x00040000
#define  PLL_INPUT_CLOCK_X_4      0x00080000
#define  PLL_INPUT_CLOCK_X_5      0x000C0000
#define  PLL_INPUT_CLOCK_X_6      0x00100000
#define  PLL_INPUT_CLOCK_X_7      0x00140000
#define  PLL_INPUT_CLOCK_X_8      0x00180000
#define  PLL_INPUT_CLOCK_X_9      0x001C0000
#define  PLL_INPUT_CLOCK_X_10     0x00200000
#define  PLL_INPUT_CLOCK_X_11     0x00240000
#define  PLL_INPUT_CLOCK_X_12     0x00280000
#define  PLL_INPUT_CLOCK_X_13     0x002C0000
#define  PLL_INPUT_CLOCK_X_14     0x00300000
#define  PLL_INPUT_CLOCK_X_15     0x00340000
#define  PLL_INPUT_CLOCK_X_16     0x00380000

#define PCLK2_DIVIDED_BY_2         0b00
#define PCLK2_DIVIDED_BY_4         0b01
#define PCLK2_DIVIDED_BY_6         0b10
#define PCLK2_DIVIDED_BY_8         0b11

#define MCOSRC_SYSTEMCLK           0b100
#define MCOSRC_HSI                 0b101
#define MCOSRC_HSE                 0b110
#define MCOSRC_PLL_DIVIDED_BY_2    0b111

#define PLL_CLOCK_DIVIDED_BY_ONE_AND_HALF	0b0
#define PLL_CLOCK_NOT_DIVIDED				0b1

#define RCC_APB2_CLOCK_PORT_A		0X00000004
#define RCC_APB2_CLOCK_PORT_B		0X00000008
#define RCC_APB2_CLOCK_PORT_C		0X00000010
#define RCC_APB2_CLOCK_UART1EN		0x00004000
#define RCC_APB2_CLOCK_SPI1EN		0X00001000

#define RCC_APB1_CLOCK_UART2EN		0x00020000
#define RCC_APB1_CLOCK_UART3EN		0x00040000

#define RCC_AHP_CLOCK_DMA1			0x00000001
#define RCC_AHP_CLOCK_DMA2			0x00000002

typedef enum{
HSI_ON,
HSE_ON,
PLL_ON
}CLOCK_TYPE;

typedef enum{
ON,
OFF
}CLOCK_STATUS;

typedef enum{
PLLSRC_HSI_DIVIDED_BY_2,
PLLSRC_HSE,
PLLSRC_HSE_DIVIDED_BY_2
}PLL_SRC;

typedef enum {
	AHP,
	APB1,
	APB2
}BUS;

/*RCC_ControlClock enables or disables one of the three clocks based on the user input, which is one of the following:
 * First argument: is an object of the CLOCK_TYPE enum with one of the following values:  [HSI_ON,HSE_ON,PLL_ON]
 * second argument is an object of the CLOCK_STATUS enum with one of the following values: [ON,OFF]
 * returns status of the operation whether it's ok or not*/
STD_ERROR RCC_ControlClock(u8 copy_u8ClockType, u8 copy_u8ClockStatus);

/*RCC_stdErrorSetSYSClock selects the system clock
 * {for HSI -> HSI_SW,  for HSE -> HSE_SW,  for PLL -> PLL_SW} */
STD_ERROR RCC_SetSYSClock(u8 copy_u8SystemClock);

/*RCC_stdErrorGetSYSClock takes a pointer to variable that will store the value of selected system clock*/
STD_ERROR RCC_GetSYSClock(u32 *copy_u8SystemClock);

/*RCC_stdErrorConfigPLL takes one of the following values for
 * first argument:
 * RCC_stdErrorSetPLLSRC chooses the clock for PLL entry. The inputs are:
 * PLLSRC_HSI_DIVIDED_BY_2
 * PLLSRC_HSE
 * PLLSRC_HSE_DIVIDED_BY_2
 *
 * second argument:
 *PLL_INPUT_CLOCK_X_N -and N is from 2 to 16 which is the value of PLL Multiplication- */
STD_ERROR RCC_ConfigPLL(u8 copy_u8PLLClockSource, u32 copy_u8PLLMultiplicationFactor);

/*RCC_stdErrorControlPerihperal enables the clock on the selected peripheral
 * the first argument is one of the following: AHP, APB1, APB2
 * the second argument is the peripheral to be selected
 * RCC_APB2_CLOCK_PORT_X -and X is one of the following A,B,C-
 * the third argument is the status of the Clock: ON or OFF*/
STD_ERROR RCC_ControlPerihperal(u8 copy_u8Bus, u32 copy_u32Peripheral, u8 status);

STD_ERROR RCC_GetPLLMUL(u32 *copy_u8SystemClock);
STD_ERROR RCC_GetPLLSRC(u32 *copy_u8SystemClock);
STD_ERROR RCC_GetPLLXTPRE(u32 *copy_u8SystemClock);
STD_ERROR RCC_GetHPRE(u32 *copy_u8SystemClock);
STD_ERROR RCC_GetPPRE1(u32 *copy_u8SystemClock);
STD_ERROR RCC_GetPPRE2(u32 *copy_u8SystemClock);
u8 RCC_GetSFTRSTF(void);

#endif /* DRCC_H_ */
