#include "STD_TYPES.h"
#define t1 (0.00111f)
#define tm (0.06352f)


/* for AVR*/
/* for loop time          t1 = 5.39875 us */
/* calculation time       t2 = 627.875 us */
/* context switching into t3 = 3.995 us */
/* context switching out  t4 = 2.255 us */

/*for ARM*/
/* this delay for 8 MHZ*/
/* for loop time          t1 =1.11  us */
/* calculation time       t2 = 138.52 us new value = 61.32 */
/* context switching into t3 = 0.76us  new value = 0.92 */
/* context switching out  t4 = 0.52us  new value = 1.28*/
/* Anis Time  tAnis = 5.48 us */
/* ARM values are not accurate */


void Delay_ms(uint32_t Value)
{
	 uint32_t i;
	 uint32_t count = (uint32_t)((float)Value-tm)/t1;

	for(i=0 ; i<count ; i++)
	{
		asm("NOP");
	}

}
