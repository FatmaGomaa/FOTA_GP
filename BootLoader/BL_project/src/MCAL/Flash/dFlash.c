/*
 * dFlash.c
 *
 *  Created on: May 5, 2020
 *      Author: FatmaGomaa
 */



#include "STD_TYPES.h"
#include "dFlash.h"

/* Flash_ACR registers Macros */

/* It is read only bit and this mask will be used for checking */
#define FLASH_ACR_PREFETCH_BUFFER_STATUS         0x00000020
#define FLASH_ACR_PREFETCH_BUFFER_ENABLE         0x00000010
#define FLASH_ACR_PREFETCH_BUFFER_DISABLE        0x00000000
#define FLASH_ACR_HALF_CYCLE_ACCESS_ENABLE       0x00000008
#define FLASH_ACR_HALF_CYCLE_ACCESS_DISABLE      0x00000000
#define FLASH_ACR_LATENCY_ZERO_WAIT              0x00000000
#define FLASH_ACR_LATENCY_ONE_WAIT               0x00000001
#define FLASH_ACR_LATENCY_TWO_WAIT               0x00000002

/* Flash_SR registers Macros */
#define FLASH_SR_END_OF_OPERATION                0x00000020
#define FLASH_SR_WRITE_PROTECTION_ERROR          0x00000010
#define FLASH_SR_PROGRAM_ERROR                   0x00000004
#define FLASH_SR_BUSY                            0x00000001

/* Flash_CR registers Macros */
#define FLASH_CR_EOPIE                           0x00001000
#define FLASH_CR_ERRIE                           0x00000400
#define FLASH_CR_OPTWRE                          0x00000200
#define FLASH_CR_LOCK                            0x00000080
#define FLASH_CR_STRT                            0x00000040
#define FLASH_CR_OPTER                           0x00000020
#define FLASH_CR_OPTPG                           0x00000010
#define FLASH_CR_MER                             0x00000004
#define FLASH_CR_PER                             0x00000002
#define FLASH_CR_PG                              0x00000001

#define FLASH_KEY1                               (u32)(0x45670123)
#define FLASH_KEY2                               (u32)(0xCDEF89AB)
#define FLASH_MAIN_MEMORY_START                  0x08000000
#define FLASH_MAIN_MEMORY_END                    0x08004000

#define WORD_SIZE								 16

typedef struct
{
	u32 FLASH_ACR;
	u32 FLASH_KEYR;
	u32 FLASH_OPTKEYR;
	u32 FLASH_SR;
	u32 FLASH_CR;
	u32 FLASH_AR;
	u32 Reserved;
	u32 FLASH_OBR;
	u32 FLASH_WRPR;

}FlashPeripheral_t;

#define FLASH_BASE_ADDRESS			(( volatile void * const ) 0x40022000)

static volatile FlashPeripheral_t * FLASH = (FlashPeripheral_t *) FLASH_BASE_ADDRESS;

/* Static in .c   */
STD_ERROR Flash_WriteWord(void * Address, u32 Data){
	STD_ERROR Local_ErrorStatus = NOT_OK;

	while(FLASH->FLASH_SR & FLASH_SR_BUSY);
	FLASH->FLASH_CR |= FLASH_CR_PG;
	*( (u16 *)Address ) = (u16)Data;

	while(FLASH->FLASH_SR & FLASH_SR_BUSY);
	FLASH->FLASH_CR |= FLASH_SR_END_OF_OPERATION;
	*( (u16 *)Address + 1 ) = (u16)(Data>>WORD_SIZE);
	while(FLASH->FLASH_SR & FLASH_SR_BUSY);

	FLASH->FLASH_CR &=~ FLASH_CR_PG;

	if (*(u32*)Address == Data){
		Local_ErrorStatus = OK;
	}
	return Local_ErrorStatus;

}

STD_ERROR Flash_ProgramWrite(volatile void * StartAddress,volatile void * DataAddress, u16 NumberOfBytes){
	volatile STD_ERROR Local_ErrorStatus = NOT_OK;
	u16 Local_Iterator = 0;
	volatile u32 ** pptr = (volatile u32*)(&StartAddress);
	if(( (u32)pptr >= FLASH_MAIN_MEMORY_START )||( (u32)pptr <= FLASH_MAIN_MEMORY_END)){
		Local_ErrorStatus = OK;
	}


	while(FLASH->FLASH_SR & FLASH_SR_BUSY);
	FLASH->FLASH_CR |= FLASH_CR_PG;

	for(Local_Iterator=0; Local_Iterator<NumberOfBytes/2; Local_Iterator++){
		*( ((u16 *)StartAddress) + Local_Iterator) =(u16) *( ((u16*)DataAddress) + Local_Iterator) ;
		while(FLASH->FLASH_SR & FLASH_SR_BUSY);
		FLASH->FLASH_CR |= FLASH_SR_END_OF_OPERATION;

		if ( (*( ((u16 *)StartAddress) + Local_Iterator) ) == ( (u16) *( ((u16*)DataAddress) + Local_Iterator) ) ){
			Local_ErrorStatus &= OK;
		}else {
			Local_ErrorStatus == NOT_OK;
			break;
		}
	}
	FLASH->FLASH_CR &=~ FLASH_CR_PG;
	return Local_ErrorStatus;

}
STD_ERROR Flash_Lock(void)
{
	STD_ERROR Local_ErrorStatus = NOT_OK;
	FLASH->FLASH_CR |= FLASH_CR_LOCK;
	if(FLASH->FLASH_CR & FLASH_CR_LOCK){
		Local_ErrorStatus = OK;
	}
	return Local_ErrorStatus;
}
STD_ERROR Flash_Unlock(void)
{
	STD_ERROR Local_ErrorStatus = NOT_OK;
	if((FLASH->FLASH_CR & FLASH_CR_LOCK) ){
		FLASH->FLASH_KEYR = FLASH_KEY1;
		FLASH->FLASH_KEYR = FLASH_KEY2;
		Local_ErrorStatus = OK;
	}
	return Local_ErrorStatus;
}
STD_ERROR Flash_ErassPage(volatile u32 PageAddress)
{
	volatile STD_ERROR Local_ErrorStatus = NOT_OK;
	/*Check that the address is within the Flash Range*/
	if((PageAddress >= FLASH_MAIN_MEMORY_START )||(PageAddress <= FLASH_MAIN_MEMORY_END)){
		Local_ErrorStatus = OK;
	}
	while(FLASH->FLASH_SR & FLASH_SR_BUSY);
	FLASH->FLASH_CR |= FLASH_CR_PER;
	FLASH->FLASH_AR = PageAddress;
	FLASH->FLASH_CR |= FLASH_CR_STRT;
	while(FLASH->FLASH_SR & FLASH_SR_BUSY);
	if(  (*((volatile u32 *)PageAddress) ) == 0xffffffff ){
		Local_ErrorStatus &= OK;
	}

	FLASH->FLASH_CR &= ~ (FLASH_CR_PER);

	return Local_ErrorStatus;
}
void Flash_EraseMass(void)
{
	while(FLASH->FLASH_SR & FLASH_SR_BUSY);
	FLASH->FLASH_CR |= FLASH_CR_MER;
	FLASH->FLASH_CR |= FLASH_CR_STRT;
	FLASH->FLASH_CR &= ~(FLASH_CR_MER);

}


