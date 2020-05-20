/*
 * dFlash.h
 *
 *  Created on: May 5, 2020
 *      Author: FatmaGomaa
 */
#ifndef MCAL_FLASH_DFLASH_H_
#define MCAL_FLASH_DFLASH_H_

/* Static in .c   */
extern STD_ERROR Flash_WriteWord(void * Address, u32 Data);
extern STD_ERROR Flash_ProgramWrite(volatile void * StartAddress, volatile void * DataAddress, u16 NumberOfBytes);
extern STD_ERROR Flash_Unlock(void);
extern STD_ERROR Flash_Lock(void);
extern STD_ERROR Flash_ErassPage(volatile u32 PageAddress);
extern void Flash_EraseMass(void);



#endif /* MCAL_FLASH_DFLASH_H_ */
