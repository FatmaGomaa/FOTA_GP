/*
 * dGPIO.h
 *
 *  Created on: Feb 8, 2020
 *      Author: FatmaGomaa
 */

#ifndef DGPIO_H_
#define DGPIO_H_

#define PORT_A		                            ((PORT_REGISTERS * )0x40010800)
#define PORT_B		                            ((PORT_REGISTERS * )0x40010C00)
#define PORT_C		                            ((PORT_REGISTERS * )0x40011000)
/*
#define DPIN_0									0
#define DPIN_1									1
#define DPIN_2									2
#define DPIN_3									3
#define DPIN_4									4
#define DPIN_5									5
#define DPIN_6									6
#define DPIN_7									7
#define DPIN_8									8
#define DPIN_9									9
#define DPIN_10									10
#define DPIN_11									11
#define DPIN_12									12
#define DPIN_13									13
#define DPIN_14									14
#define DPIN_15									15
*/
#define PIN_0									0x0001
#define PIN_1									0x0002
#define PIN_2									0x0004
#define PIN_3									0x0008
#define PIN_4									0x0010
#define PIN_5									0x0020
#define PIN_6									0x0040
#define PIN_7									0x0080
#define PIN_8									0x0100
#define PIN_9									0x0200
#define PIN_10									0x0400
#define PIN_11									0x0800
#define PIN_12									0x1000
#define PIN_13									0x2000
#define PIN_14									0x4000
#define PIN_15									0x8000

typedef enum{
	INPUT_ANALOG_MODE							=0B0000,
	INPUT_FLOATING_MODE							=0B0100,
	INPUT_PULLUP_PULLDOWN_MODE					=0B1000,

	OUTPUT_GP_PUSH_PULL_10MHZ					=0B0001,
	OUTPUT_GP_PUSH_PULL_2MHZ					=0B0010,
	OUTPUT_GP_PUSH_PULL_50MHZ					=0B0011,

	OUTPUT_GP_OPEN_DRAIN_10MHZ                  =0B0101,
	OUTPUT_GP_OPEN_DRAIN_2MHZ                   =0B0110,
	OUTPUT_GP_OPEN_DRAIN_50MHZ                  =0B0111,

	OUTPUT_AF_PUSH_PULL_10MHZ                   =0B1001,
	OUTPUT_AF_PUSH_PULL_2MHZ                    =0B1010,
	OUTPUT_AF_PUSH_PULL_50MHZ                   =0B1011,

	OUTPUT_AF_OPEN_DRAIN_10MHZ                  =0B1101,
	OUTPUT_AF_OPEN_DRAIN_2MHZ                   =0B1110,
	OUTPUT_AF_OPEN_DRAIN_50MHZ                  =0B1111,

}GPIO_PIN_MOOD;


typedef struct{
	u32 GPIO_CRL;
	u32 GIPO_CRH;
	u32 GPIO_IDR;
	u32 GIPO_ODR;
	u32 GPIO_BSRR;
	u32 GIPO_BRR;
	u32 GIPO_LCKR;
}PORT_REGISTERS;

typedef struct{
	volatile void* PORT;
	u32 PIN;
	u32 MOOD;

}GPIO_t;

/*The function takes a pointer to structure, the structure has
 * 3 main elements that the user chooses
 * 1. PORT in range PORT_A, PORT_B, PORT_C
 * 2. PIN in range PIN_0 .... PIN_15 or multiple pins through ORING them
 * 3. MOOD in range INPUT_X to OUTPUT_X
 *
 * and it returns Standard error whether the function did it job or failed*/
STD_ERROR GPIO_Config(GPIO_t * GPIO_Config);

/*The function takes 3 arguments PORT, PIN and output value to be written on the pin, the arguments have on of the following values
 * 1. PORT in range PORT_A, PORT_B, PORT_C
 * 2. PIN in range PIN_0 .... PIN_15
 * 3. OuputValue in range HIGH or LOW
 * and it returns Standard error whether the function did it job or failed*/
STD_ERROR GPIO_writePin(volatile void* PORT, u32 PIN, u8 OutputValue);

/*The function takes a pointer to the Port and output value to be written on the port.
 * the port has the following options:
 * PORT_X and X in range [A,B,C]
 * and it returns Standard error whether the function did it job or failed*/
STD_ERROR GPIO_writePort(volatile void* PORT, u8 OutputValue);
/*The function returns the value of the input pin on the specified PORT
 * 1. PORT in range PORT_A, PORT_B, PORT_C
 * 2. PIN in range PIN_0 .... PIN_15*/
u8 GPIO_u8getPinValue(volatile void* PORT, u32 PIN);

#endif /* DGPIO_H_ */


