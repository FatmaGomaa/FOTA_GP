#include "STD_TYPES.h"
#include "DELAY.h"
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "cmsis_device.h"


uint8_t MyMarker __attribute__((section(".marker"),used));

//typedef void (*APP_Entrypoint_t)(void) ;
//APP_Entrypoint_t APP_Entrypoint __attribute__((section(".app"),used));

//const APP_Entrypoint_t APP_Entrypoint = (APP_Entrypoint_t) 0x08003004;//0x080038e0;//0x0800310C;


static void start_app(uint32_t pc, uint32_t sp) __attribute__((naked));


static void start_app(uint32_t pc, uint32_t sp)
{
	asm("msr msp, r1 ");
	asm(" bx r0");
}

#define __approm_start__	0x08003000

int main()
{

	RCC_SelectSystemClk(RCC_SYS_CLK_HSE);

	RCC_APB2_SetPeripheralClk(RCC_APB2_CLK_PORTC , RCC_STAT_ON);
	GPIO_InitOnePin(PORTC,PIN13 , MODE_OUTPUT_10_GP_PUSHPULL);

	Delay_ms(1000);

	SCB->VTOR = 0x00003000 << 9;



	uint32_t *app_code = (uint32_t *)__approm_start__;
	uint32_t app_sp = app_code[0];
	uint32_t app_start = app_code[1];
	start_app(app_start, app_sp);

//	APP_Entrypoint();  // by this method the program counter is not updated right why ?!!!!!!!!!!!!!!!!!!!

	while(1)
	{

	}
	return 0;
}




/*
 *    APP(rx) : ORIGIN = 0x08003000, LENGTH = 20k
 *
 *
 *    .app : ALIGN(4)
    {

        *(.app .app.*)
        KEEP(*(.app*))


    } >APP
 * */




/* MARKER (rx) : ORIGIN = 0x0800F000, LENGTH = 4
 *
 *
 *
 *     .marker : ALIGN(4)
    {


        *(.marker .marker.*)
        KEEP(*(.marker*))


    } >MARKER
 *
 *
 * */
