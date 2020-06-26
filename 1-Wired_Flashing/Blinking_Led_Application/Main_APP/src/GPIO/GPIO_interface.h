/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * File name: GPIO_interface.h                                                                         *
 *                                                                                                     *
 * Description: This Header file is used to interface with the GPIO module in stm32f10x evaluation kit *
 *                                                                                                     *
 * Author : Mohamed Adel Anis Ahmed				                                                       *
 * date   : 15 MAY 2020                                                                                *
 * version: 2.0                                                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_

/*====================================================================================================*/
/****************************************Macros & Definitions******************************************/
/*====================================================================================================*/

/* Base Address for Ports' Registers */
#define PORTA	                            ((void*)(0x40010800))
#define PORTB	                            ((void*)(0x40010C00))
#define PORTC	                            ((void*)(0x40011000))

/*#defines for Pins */
#define P_NUM0                              (0x0001)
#define P_NUM1                              (0x0002)
#define P_NUM2                              (0x0004)
#define P_NUM3                              (0x0008)
#define P_NUM4                              (0x0010)
#define P_NUM5                              (0x0020)
#define P_NUM6                              (0x0040)
#define P_NUM7                              (0x0080)
#define P_NUM8                              (0x0100)
#define P_NUM9                              (0x0200)
#define P_NUM10	                            (0x0400)
#define P_NUM11	                            (0x0800)
#define P_NUM12	                            (0x1000)
#define P_NUM13	                            (0x2000)
#define P_NUM14	                            (0x4000)
#define P_NUM15	                            (0x8000)


#define PIN0								(0)
#define PIN1	                            (1)
#define PIN2	                            (2)
#define PIN3	                            (3)
#define PIN4	                            (4)
#define PIN5	                            (5)
#define PIN6	                            (6)
#define PIN7	                            (7)
#define PIN8	                            (8)
#define PIN9	                            (9)
#define PIN10                               (10)
#define PIN11                               (11)
#define PIN12                               (12)
#define PIN13                               (13)
#define PIN14                               (14)
#define PIN15                               (15)

/* #defines for MODES */

#define MODE_INPUT_ANALOG					(0b0000)
#define MODE_INPUT_FLOATING					(0b0100)
#define MODE_INPUT_PULLUP					(0b1000)
#define MODE_INPUT_PULLDOWN					(0b1101)

#define MODE_OUTPUT_10_GP_PUSHPULL			(0b0001)
#define MODE_OUTPUT_10_GP_OPENDRAIN			(0b0101)
#define MODE_OUTPUT_10_AF_PUSHPULL			(0b1001)
#define MODE_OUTPUT_10_AF_OPENDRAIN			(0b1101)

#define MODE_OUTPUT_2_GP_PUSHPULL			(0b0010)
#define MODE_OUTPUT_2_GP_OPENDRAIN			(0b0110)
#define MODE_OUTPUT_2_AF_PUSHPULL			(0b1010)
#define MODE_OUTPUT_2_AF_OPENDRAIN			(0b1110)

#define MODE_OUTPUT_50_GP_PUSHPULL			(0b0011)
#define MODE_OUTPUT_50_GP_OPENDRAIN			(0b0111)
#define MODE_OUTPUT_50_AF_PUSHPULL			(0b1011)
#define MODE_OUTPUT_50_AF_OPENDRAIN			(0b1111)


typedef struct{

	void* PORT;
	uint8_t PIN;
	uint8_t MODE;

}GPIO_PinConfig_t;


/*====================================================================================================*/
/*********************************************Miscellaneous********************************************/
/*====================================================================================================*/
#define HIGH	(1)
#define LOW		(0)

/*====================================================================================================*/
/*********************************************Error Types**********************************************/
/*====================================================================================================*/
typedef uint8_t GPIO_ErrorStatus_t;

#define GPIO_ERR_NOK				(0)
#define GPIO_ERR_OK					(1)


/*====================================================================================================*/
/*********************************************Prototypes***********************************************/
/*====================================================================================================*/

/* The next prototypes are used by pointer to structure method for the pre-linking method*/

GPIO_ErrorStatus_t GPIO_InitStr(GPIO_PinConfig_t* GPIO_PinConfig);
GPIO_ErrorStatus_t GPIO_SetPinValue(GPIO_PinConfig_t* GPIO_PinConfig , uint8_t Value);
GPIO_ErrorStatus_t GPIO_GetPinValue(GPIO_PinConfig_t* GPIO_PinConfig , uint8_t* Value);

GPIO_ErrorStatus_t GPIO_InitOnePin(void* PORT , uint8_t PIN , uint8_t MODE);


/* The next prototypes used to initialize multiple pins in the same time at the same port */
GPIO_ErrorStatus_t GPIO_InitPins(void* PORT , uint16_t P_NUM , uint8_t MODE);
GPIO_ErrorStatus_t GPIO_writePinValue(void* PORT , uint16_t P_NUM , uint8_t Value);
GPIO_ErrorStatus_t GPIO_ReadPinValue(void* PORT , uint16_t P_NUM , uint8_t* Value);

GPIO_ErrorStatus_t GPIO_writePortValue(void* PORT , uint16_t PortValue);


#endif /* GPIO_INTERFACE_H_ */
