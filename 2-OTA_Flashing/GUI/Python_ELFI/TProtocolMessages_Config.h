/*
 * TProtocolMessages_Config.h
 *
 *  Created on: Apr 9, 2020
 *      Author: FatmaGomaa
 */

#ifndef SERVICE_TRANSPORTPROTOCOL_TPROTOCOLMESSAGES_CONFIG_H_
#define SERVICE_TRANSPORTPROTOCOL_TPROTOCOLMESSAGES_CONFIG_H_

#include "TProtocolMessages_Config.h"
#include "TProtocol.h"


typedef enum{
	ID_EraseCommand	 		= 0x45,
	ID_ResponseCommand   	= 0x52,
	ID_DataCommand			= 0x44,
	ID_VerifyCommand		= 0x46,
	ID_MarkerCommand        = 0x54
}Message_ID;

/*TODO: to add R_NOT types*/
typedef enum{
	R_OK,
	R_NOT_SectionOffestViolation,
	R_NOT_SectionsOutOfScope,
	R_NOT_MismatchData,
	R_NOT_EraseFailure,
	R_NOT_SAMEAPPLICATION,
	R_NOT_TIMEOUT
}Response_t;

typedef struct{
	u16 SectionsCount;
	u16 SectionOffset;
	u8 CheckSum;
}EraseCommand_t;

typedef struct{
	u8 	Response;
	u32 Reserved;
}ResponseCommand_t;

typedef struct{
	u8 Data[FRAME_DATA_BYTES];
}DataCommand_t;

typedef struct{
	u32 CheckSum;
	u8  Reserved;
}VerifyCommand_t;

typedef struct{
	u32 marker;
	u8  Reserved;
}MarkerCommand_t;


#endif /* SERVICE_TRANSPORTPROTOCOL_TPROTOCOLMESSAGES_CONFIG_H_ */
