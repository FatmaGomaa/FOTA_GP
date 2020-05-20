/*
 * DINTERRUPTS_program.c
 *
 *  Created on: Feb 25, 2020
 *      Author: mayab
 */

#include <stdint-gcc.h>

#include "STD_Types.h"
#include "DNVIC_interface.h"

#define SCB_AIRCR   ((u32 volatile * const )0xE000ED0C)

#define SETENA      ((u32 volatile * const )0xE000E100)
#define CLRENA      ((u32 volatile * const )0xE000E180)

#define SETPEND     ((u32 volatile * const )0xE000E200)
#define CLRPEND     ((u32 volatile * const )0xE000E280)

#define ACTIVE      ((u32 volatile * const )0xE000E300)

#define PRIORITY    ((u8 volatile * const )0xE000E400)

#define MAX_REG_BITS  32


void DNVIC_SetPriorityGrouping(u32 priority_grouping)
{
	*SCB_AIRCR = priority_grouping;
}


void DNVIC_EnableIRQ(u8 interrupt_no)
{

	u8 index = interrupt_no/MAX_REG_BITS;
	u8 bit_num = interrupt_no%MAX_REG_BITS;

	*(SETENA + index) = (1<<bit_num);

}
void DNVIC_DisableIRQ(u8 interrupt_no)
{

	u8 index = interrupt_no/MAX_REG_BITS;
	u8 bit_num = interrupt_no%MAX_REG_BITS;

	*(CLRENA + index) = (1<<bit_num);

}

//stdError NVIC_GetPendingIRQ (u8 interrupt_no, u8 )

void DNVIC_SetPending(u8 interrupt_no)
{
	u8 index = interrupt_no/MAX_REG_BITS;
	u8 bit_num = interrupt_no%MAX_REG_BITS;
	*(SETPEND + index) = (1<<bit_num);
}

void DNVIC_ClearPending(u8 interrupt_no)
{
	u8 index = interrupt_no/MAX_REG_BITS;
	u8 bit_num = interrupt_no%MAX_REG_BITS;
	*(CLRPEND + index) = (1<<bit_num);

}

/*An API to return all the */
void DNVIC_GetActive (u8 interrupt_no, u32 * result)
{
	u8 index = interrupt_no/MAX_REG_BITS;
	u8 bit_num = interrupt_no%MAX_REG_BITS;

	if((*(ACTIVE + index)) &= ((1<<bit_num)))
	{
		*result = ACTIVE_INTERRUPT;
	}

	else
	{
		*result = NOT_ACTIVE_INTERRUPT;
	}


}

void NVIC_GetPendingIRQ (u8 interrupt_no, u32 * result)
{
	u8 index = interrupt_no/MAX_REG_BITS;
	u8 bit_num = interrupt_no%MAX_REG_BITS;

	if((*(SETPEND + index)) &= ((1<<bit_num)))
	{
		*result = PENDING_INTERRUPT;
	}

	else
	{
		*result = NOT_PENDING_INTERRUPT;
	}
}


void DNVIC_SetPriority (u8 interrupt_no, u32 priority)
{
	*(PRIORITY + interrupt_no) = priority<<4;
}
void DNVIC_GetPriority (u8 interrupt_no, u32 * result)
{

}


void NVIC_EnablePRIMASK(void)
{
	asm("MOV R0, #1");
	asm("MSR PRIMASK, R0");
}

void NVIC_DisablePRIMASK(void)
{
	asm("MOV R0, #0");
	asm("MSR PRIMASK, R0");
}

void NVIC_EnableFAULTMASK(void)
{
	asm("MOV R0, #1");
	asm("MSR FAULTMASK, R0");
}

void NVIC_DisableFAULTMASK(void)
{
	asm("MOV R0, #0");
	asm("MSR FAULTMASK, R0");
}

void NVIC_EnableBASEPRI(u8 priority)
{
	asm ("MSR BASEPRI, R0");
}

void NVIC_DisableBASEPRI(void)
{
	asm("MOV R0, #0");
	asm ("MSR BASEPRI, R0");
}
