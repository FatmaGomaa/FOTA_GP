#include "STD_TYPES.h"
#include "DELAY.h"
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "cmsis_device.h"


#define  Marker          	*(volatile u32 *)(0x20000000)
#define  Flag            	*(volatile u32 *)(0x20000004)


#define DIO_SIGNAL_PORT      	PORTA
#define DIO_SIGNAL_PIN       	P_NUM1

int main()
{
	u8 FlashNewAppDIO = 0;

	RCC_SelectSystemClk(RCC_SYS_CLK_HSE);
	RCC_APB2_SetPeripheralClk(RCC_APB2_CLK_PORTA , RCC_STAT_ON);

	GPIO_InitOnePin(PORTA,PIN0 , MODE_OUTPUT_10_GP_PUSHPULL);

	while(1)
	{
		GPIO_writePinValue(PORTA , P_NUM0 , HIGH);
		Delay_ms(500);
		GPIO_writePinValue(PORTA , P_NUM0 , LOW);
		Delay_ms(500);

		 GPIO_ReadPinValue(DIO_SIGNAL_PORT,DIO_SIGNAL_PIN,&FlashNewAppDIO);
		if( FlashNewAppDIO == 1)
		{
			/*Requesting Software Reset */

				SCB->AIRCR = (0x5FA0000 | ( 1 << 2 ));
		}

	}
}
