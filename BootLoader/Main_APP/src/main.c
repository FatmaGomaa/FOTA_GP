#include "STD_TYPES.h"
#include "DELAY.h"
#include "RCC_interface.h"
#include "GPIO_interface.h"

int main()
{
	RCC_SelectSystemClk(RCC_SYS_CLK_HSE);
	RCC_APB2_SetPeripheralClk(RCC_APB2_CLK_PORTA , RCC_STAT_ON);

	GPIO_InitOnePin(PORTA,PIN0 , MODE_OUTPUT_10_GP_PUSHPULL);

	while(1)
	{

		GPIO_writePinValue(PORTA , P_NUM0 , HIGH);
		Delay_ms(500);
		GPIO_writePinValue(PORTA , P_NUM0 , LOW);
		Delay_ms(500);

	}
}
