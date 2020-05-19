#include "STD_TYPES.h"
#include "DELAY.h"
#include "RCC_interface.h"
#include "GPIO_interface.h"

int main()
{
	RCC_SelectSystemClk(RCC_SYS_CLK_HSE);
	RCC_APB2_SetPeripheralClk(RCC_APB2_CLK_PORTC , RCC_STAT_ON);

	GPIO_InitOnePin(PORTC,PIN13 , MODE_OUTPUT_10_GP_PUSHPULL);

	while(1)
	{

		GPIO_writePinValue(PORTC , P_NUM13 , HIGH);
		Delay_ms(1000);
		GPIO_writePinValue(PORTC , P_NUM13 , LOW);
		Delay_ms(1000);

	}
}
