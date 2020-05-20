/*
 * DINTERRUPTS_interface.h
 *
 *  Created on: Feb 25, 2020
 *      Author: mayab
 */




#ifndef DNVIC_INTERFACE_H_
#define DNVIC_INTERFACE_H_

/* All the Interrupts enabled in ST */
#define WWDG				0
#define PVD					1
#define TAMPER				2
#define RTC					3
#define FLASH				4
#define RCCINT				5
#define EXTI0				6
#define EXTI1				7
#define EXTI2				8
#define EXTI3				9
#define EXTI4				10
#define DMA1_CHANNEL1		11
#define DMA1_CHANNEL2		12
#define DMA1_CHANNEL3		13
#define DMA1_CHANNEL4		14
#define DMA1_CHANNEL5		15
#define DMA1_CHANNEL6		16
#define DMA1_CHANNEL7		17
#define ADC1_2				18
#define USB_HP_CAN_TX		19
#define USB_LP_CAN_RX0		20
#define CAN_RX1				21
#define CAN_SCE				22
#define EXTI9_5				23
#define TIM1_BRK			24
#define TIM1_UP				25
#define TIM1_TRG_COM		26
#define TIM1_CC				27
#define TIM2				28
#define TIM3				29
#define TIM4				30
#define I2C1_EV				31
#define I2C1_ER				32
#define I2C2_EV				33
#define I2C2_ER				34
#define SPI1				35
#define SPI2				36
#define USART1				37
#define USART2				38
#define USART3				39
#define EXTI15_10			40
#define RTCALARM			41
#define USBWAKEUP			42
#define TIM8_BRK			43
#define TIM8_UP				44
#define TIM8_TRG_COM		45
#define TIM8_CC				46
#define ADC3				47
#define FSMC				48
#define SDIO				49
#define TIM5				50
#define SPI3				51
#define UART4				52
#define UART5				53
#define TIM6				54
#define TIM7				55
#define DMA2_CHANNEL1		56
#define DMA2_CHANNEL2		57
#define DMA2_CHANNEL3		58
#define DMA2_CHANNEL4_5		59


/* The number of pre-emptions bits to the number of the sub-priority bits*/
#define GROUP_PRIO_4_SUBPRIO_0     0x05FA0300
#define GROUP_PRIO_3_SUBPRIO_1     0x05FA0400
#define GROUP_PRIO_2_SUBPRIO_2     0x05FA0500
#define GROUP_PRIO_1_SUBPRIO_3     0x05FA0600
#define GROUP_PRIO_0_SUBPRIO_4     0x05FA0700



/* The returns of some API's */
#define ACTIVE_INTERRUPT           1
#define NOT_ACTIVE_INTERRUPT       0
#define PENDING_INTERRUPT          1
#define NOT_PENDING_INTERRUPT      0


/* The priorities to choose from, for a particular interrupt  */
#define INTERRUPT_PRIOPITY_0       0
#define INTERRUPT_PRIOPITY_1       1
#define INTERRUPT_PRIOPITY_2       2
#define INTERRUPT_PRIOPITY_3       3
#define INTERRUPT_PRIOPITY_4       4
#define INTERRUPT_PRIOPITY_5       5
#define INTERRUPT_PRIOPITY_6       6
#define INTERRUPT_PRIOPITY_7       7
#define INTERRUPT_PRIOPITY_8       8
#define INTERRUPT_PRIOPITY_9       9
#define INTERRUPT_PRIOPITY_10      10
#define INTERRUPT_PRIOPITY_11      11
#define INTERRUPT_PRIOPITY_12      12
#define INTERRUPT_PRIOPITY_13      13
#define INTERRUPT_PRIOPITY_14      14
#define INTERRUPT_PRIOPITY_15      15

/* Sets the priority percentage pre-emption to sub-priority
 * Inputs :  GROUP_PRIO_4_SUBPRIO_0, GROUP_PRIO_3_SUBPRIO_1, GROUP_PRIO_2_SUBPRIO_2, GROUP_PRIO_1_SUBPRIO_3, GROUP_PRIO_0_SUBPRIO_4 */
void DNVIC_SetPriorityGrouping(u32 priority_grouping);

/*Enables a particular interrupt
 * Inputs: WWDG , PVD , TAMPER , RTC , FLASH , RCCINT ,
 * EXTI0 , EXTI1 , EXTI2 , EXTI3 , EXTI4 , DMA1_CHANNEL1 ,
 * DMA1_CHANNEL2 , DMA1_CHANNEL3 , DMA1_CHANNEL4 ,
 * DMA1_CHANNEL5 , DMA1_CHANNEL6 , DMA1_CHANNEL7 ,
 * ADC1_2 , USB_HP_CAN_TX , USB_LP_CAN_RX0 , CAN_RX1 ,
 * CAN_SCE , EXTI9_5 , TIM1_BRK , TIM1_UP , TIM1_TRG_COM ,
 * TIM1_CC , TIM2 , TIM3 , TIM4 , I2C1_EV , I2C1_ER ,
 * I2C2_EV , I2C2_ER , SPI1 , SPI2 , USART1 , USART2 ,
 * USART3 , EXTI15_10 , RTCALARM , USBWAKEUP , TIM8_BRK ,
 * TIM8_UP , TIM8_TRG_COM , TIM8_CC , ADC3 , FSMC , SDIO ,
 * TIM5 , SPI3 , UART4 , UART5 , TIM6 , TIM7 , DMA2_CHANNEL1 ,
 * DMA2_CHANNEL2 , DMA2_CHANNEL3 , DMA2_CHANNEL4_5	 */
void DNVIC_EnableIRQ(u8 interrupt_no);

/*Disables a particular interrupt
 * Inputs: WWDG , PVD , TAMPER , RTC , FLASH , RCCINT ,
 * EXTI0 , EXTI1 , EXTI2 , EXTI3 , EXTI4 , DMA1_CHANNEL1 ,
 * DMA1_CHANNEL2 , DMA1_CHANNEL3 , DMA1_CHANNEL4 ,
 * DMA1_CHANNEL5 , DMA1_CHANNEL6 , DMA1_CHANNEL7 ,
 * ADC1_2 , USB_HP_CAN_TX , USB_LP_CAN_RX0 , CAN_RX1 ,
 * CAN_SCE , EXTI9_5 , TIM1_BRK , TIM1_UP , TIM1_TRG_COM ,
 * TIM1_CC , TIM2 , TIM3 , TIM4 , I2C1_EV , I2C1_ER ,
 * I2C2_EV , I2C2_ER , SPI1 , SPI2 , USART1 , USART2 ,
 * USART3 , EXTI15_10 , RTCALARM , USBWAKEUP , TIM8_BRK ,
 * TIM8_UP , TIM8_TRG_COM , TIM8_CC , ADC3 , FSMC , SDIO ,
 * TIM5 , SPI3 , UART4 , UART5 , TIM6 , TIM7 , DMA2_CHANNEL1 ,
 * DMA2_CHANNEL2 , DMA2_CHANNEL3 , DMA2_CHANNEL4_5	 */
void DNVIC_DisableIRQ(u8 interrupt_no);

/*Set the pending flag for particular interrupt
 * Inputs: WWDG , PVD , TAMPER , RTC , FLASH , RCCINT ,
 * EXTI0 , EXTI1 , EXTI2 , EXTI3 , EXTI4 , DMA1_CHANNEL1 ,
 * DMA1_CHANNEL2 , DMA1_CHANNEL3 , DMA1_CHANNEL4 ,
 * DMA1_CHANNEL5 , DMA1_CHANNEL6 , DMA1_CHANNEL7 ,
 * ADC1_2 , USB_HP_CAN_TX , USB_LP_CAN_RX0 , CAN_RX1 ,
 * CAN_SCE , EXTI9_5 , TIM1_BRK , TIM1_UP , TIM1_TRG_COM ,
 * TIM1_CC , TIM2 , TIM3 , TIM4 , I2C1_EV , I2C1_ER ,
 * I2C2_EV , I2C2_ER , SPI1 , SPI2 , USART1 , USART2 ,
 * USART3 , EXTI15_10 , RTCALARM , USBWAKEUP , TIM8_BRK ,
 * TIM8_UP , TIM8_TRG_COM , TIM8_CC , ADC3 , FSMC , SDIO ,
 * TIM5 , SPI3 , UART4 , UART5 , TIM6 , TIM7 , DMA2_CHANNEL1 ,
 * DMA2_CHANNEL2 , DMA2_CHANNEL3 , DMA2_CHANNEL4_5	 */
void DNVIC_SetPending(u8 interrupt_no);

/*Clear the pending flag for particular interrupt
 * Inputs: WWDG , PVD , TAMPER , RTC , FLASH , RCCINT ,
 * EXTI0 , EXTI1 , EXTI2 , EXTI3 , EXTI4 , DMA1_CHANNEL1 ,
 * DMA1_CHANNEL2 , DMA1_CHANNEL3 , DMA1_CHANNEL4 ,
 * DMA1_CHANNEL5 , DMA1_CHANNEL6 , DMA1_CHANNEL7 ,
 * ADC1_2 , USB_HP_CAN_TX , USB_LP_CAN_RX0 , CAN_RX1 ,
 * CAN_SCE , EXTI9_5 , TIM1_BRK , TIM1_UP , TIM1_TRG_COM ,
 * TIM1_CC , TIM2 , TIM3 , TIM4 , I2C1_EV , I2C1_ER ,
 * I2C2_EV , I2C2_ER , SPI1 , SPI2 , USART1 , USART2 ,
 * USART3 , EXTI15_10 , RTCALARM , USBWAKEUP , TIM8_BRK ,
 * TIM8_UP , TIM8_TRG_COM , TIM8_CC , ADC3 , FSMC , SDIO ,
 * TIM5 , SPI3 , UART4 , UART5 , TIM6 , TIM7 , DMA2_CHANNEL1 ,
 * DMA2_CHANNEL2 , DMA2_CHANNEL3 , DMA2_CHANNEL4_5	 */
void DNVIC_ClearPending(u8 interrupt_no);


void DNVIC_GetActive (u8 interrupt_no, u32 * result);


void DNVIC_SetPriority (u8 interrupt_no, u32 priority);
void DNVIC_GetPriority (u8 interrupt_no, u32 * result);


void NVIC_EnablePRIMASK(void);
void NVIC_DisablePRIMASK(void);
void NVIC_EnableFAULTMASK(void);
void NVIC_DisableFAULTMASK(void);
void NVIC_EnableBASEPRI(u8 priority);
void NVIC_DisableBASEPRI(void);

#endif /* DNVIC_INTERFACE_H_ */
