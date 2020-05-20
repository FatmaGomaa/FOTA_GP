/*
 * TProtocol.c
 *
 *  Created on: Apr 9, 2020
 *      Author: FatmaGomaa
 */


#include "STD_TYPES.h"
#include "TProtocolMessages_Config.h"
#include "TProtocol.h"

#define FRAME_HEADER_START				'A'
#define END_OF_FRAME					'M'

#define TP_HEADER_LENGHT				2

void TProtcol_sendFrame(void* Data, u8* RawTrasnmitterBuffer, u8 MessageId){
	ProtocolFrame_t* Frame = (ProtocolFrame_t*)RawTrasnmitterBuffer;
	u16 Iteration;

	Frame->Header.Start = FRAME_HEADER_START;
	Frame->Header.ID = MessageId;

	for(Iteration =0; Iteration< FRAME_DATA_BYTES; Iteration++){
		Frame->Data[Iteration] = *(u8*)(Data+Iteration);
	}

	Frame->EOF = END_OF_FRAME;

}

void TProtocol_ReceiveFrame( u8* RawReceiveBuffer, void* Data, u8* MessageID){
	ProtocolFrame_t* Frame = (ProtocolFrame_t*)RawReceiveBuffer;
	u8 Iteration;

	/*Command is received correctly*/
	if ( Frame->Header.Start == FRAME_HEADER_START ){
		/*the desired message is received */
			*MessageID =  Frame->Header.ID  ;

			for(Iteration =0; Iteration< FRAME_DATA_BYTES; Iteration++){
				*(u8*)(Data + Iteration ) = Frame->Data[Iteration];
			}

	}


}


