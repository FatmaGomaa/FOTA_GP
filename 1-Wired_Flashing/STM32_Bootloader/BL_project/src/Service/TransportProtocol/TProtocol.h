/*
 * TProtocol.h
 *
 *  Created on: Apr 9, 2020
 *      Author: FatmaGomaa
 */

#ifndef SERVICE_TRANSPORTPROTOCOL_TPROTOCOL_H_
#define SERVICE_TRANSPORTPROTOCOL_TPROTOCOL_H_



/*this Macro includes  Data Bytes*/
#define FRAME_DATA_BYTES						5
/*Whole Protocol number of bytes*/
#define PROTOCOL_DATA_BYES						8

typedef struct{
	u8 Start;
	u8 ID;
}TProtocolHeader_t;

typedef struct{
	TProtocolHeader_t Header;
	u8 Data[FRAME_DATA_BYTES];
	u8 EOF;
}ProtocolFrame_t;


void TProtcol_sendFrame(void* Data, u8* RawTrasnmitterBuffer, u8  MessageId);
void TProtocol_ReceiveFrame( u8* RawReceiveBuffer,void* Data,  u8* MessageID);


#endif /* SERVICE_TRANSPORTPROTOCOL_TPROTOCOL_H_ */
