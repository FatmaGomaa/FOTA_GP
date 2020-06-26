/****************************************** Includes ************************************************************/
#include <Windows.h>
#include <stdio.h>
#include "elf_parser.h"
#include "STD_TYPES.h"
#include "TProtocol.h"
#include "TProtocolMessages_Config.h"

/******************************************** Defines ***********************************************************/

#define DATA_BLOCK_SIZE					1000         // Data block size to be flashed at a time  

/************************************** Functions prototypes ***************************************************/

void InitSerialPort(void);
void SetTimeOut(void);
void sendCommand(char * command);
void receiveCommand(void);
void closePort(void);
void openPort(void);

/************************************** Global variables ******************************************************/

HANDLE hComm;                                                     // Handle to the Serial port
unsigned char * ComPortName;                                      // Name of the Serial port passed from GUI
BOOL  Status;                                                     // Status of the various operations 
DWORD dwEventMask;                                                // Event mask to trigger from type DWORD (unsigned long)
unsigned char  TempChar;                                          // Temperory Character
unsigned char  SerialBuffer[256];                                 // Buffer Containing Rxed Data
unsigned char * lpBuffer;		       		                      // Buffer used for data written on the port  
DWORD  dNoOFBytestoWrite;                                         // No of bytes to write into the port
DWORD  dNoOfBytesWritten = 0;                                     // No of bytes written to the port
DWORD NoBytesRead;                                                // Bytes read by ReadFile()
unsigned char CommandToSend[8] = {0};			                  // Buffer holding the Frame to be sent or received (Frame Size + Null Terminator)
unsigned long long z = 0,x = 0;						              // Iterators used for producing delay
long long i = 0;                                                  // Iterators used in looping 
unsigned int m = 0;                                               // Iterators used in looping


unsigned char ProgramData[1024*1024] = {0};		                  // 60 K Program Data Buffer, will contain the whole elf file
unsigned char ProgramDataToSend[30*1024] = {0};	                  // 30 K only Program Header Sections, will contain the data of the program 
unsigned char * ElfFileName;                                      // Pointer to hold the elf file name passed from the GUI
unsigned char * MCU_Type;                                         // Pointer to hold the MCU type passed from the GUI
FILE * ElfFileDescriptor;                                         // Discriptor used to deal with the elf file
FILE * ProgressFileDescriptor;                                    // Discriptor used to deal with the progress file
FILE * tstFileDescritor;
unsigned char ProgressFileName[] = "..\\GUI\\progress.txt";       // Progress file set as interface between CommReceiver and the GUI progress bar
unsigned char ProgressFileName2[] = "tst.txt";
unsigned long size = 0;                                           // Elf file size
unsigned static long SentDataIDX = 0;                             // Index of the data in ProgramDataToSend buffer
unsigned static long SentDataBlockIDX = 0;                        // Index to the number of data blocks sent to the BL to be flashed
unsigned static long CheckSum = 0;                                // Checksum for Erase frame and for the verify Frame
unsigned char MessageID = 0;                                      // Eash frame will have a different Message ID
unsigned long lastSavedIDX = 0;                                   // Index for last send (saved) data 
unsigned long ISR_Offset = 0;                                     // ISR offset holds base address of the program in the physical memory
unsigned char Program_Size = 0;                                   // Program size in flash in sections (blockes)
unsigned long txtindex=0; 													          
Elf32_Ehdr * Header = NULL;                                       // Pointer to the elf file header
Elf32_Phdr * ProgramHeader = NULL;                                // Pointe to the program header in the elf file 
Elf32_Shdr * SectionHeader = NULL;                                // Pointe to the section header in the elf file 
#define ElfFileName "Main_APP.elf"

void main(int argc, char *argv[])
{
	/* open the elf file with read stream of bytes permission */
	ElfFileDescriptor = fopen(ElfFileName, "rb"); 
	tstFileDescritor = fopen(ProgressFileName2 , "w");
	
	/* Initialization of used Commands in Code */	
	EraseCommand_t EraseCommand = {0};
	ResponseCommand_t ResponseCommand = {0};
	DataCommand_t DataCommand = {0};
	VerifyCommand_t VerifyCommand = {0};
    
	/* Initialize the buffer that will hold the program with 0xFF, so if (mem_size - file_size > 0) the extra bytes will be 0xFF */
	long long ProgramIterator = 0;
	for(ProgramIterator = 0;ProgramIterator < sizeof(ProgramDataToSend); ProgramIterator++){
		ProgramDataToSend[ProgramIterator] = 0xFF;
	}
  
	/* Check if the file was successfully opend */
	if (ElfFileDescriptor == NULL){
		
		/* print a message on the console if it was unsuccessfully opend */
		printf("Error while opening the file.\n");
		
	}else{
		
		/* If successfully opend */
		/* Get size of Elf File */
		fseek(ElfFileDescriptor, 0, SEEK_END);
		size = ftell(ElfFileDescriptor);
		fseek(ElfFileDescriptor, 0, SEEK_SET);
		rewind(ElfFileDescriptor);


		/* Save elf file content in ProgramData buffer */
		/* fgetc reads single character at a time and moves the file pointer position to the next address */
		char ch = fgetc(ElfFileDescriptor);
		while (i != size){
			ProgramData[i]=ch;
			i++;
			ch = fgetc(ElfFileDescriptor);
		}

		/* Close elf file to open the output file */
		fclose(ElfFileDescriptor);
		
		/* Point to the elf Header, program header, section header in the buffer holding the elf file */
		Header = (Elf32_Ehdr *) &ProgramData[0];
		ProgramHeader = (Elf32_Phdr *)(&ProgramData[0] + Header->e_phoff);  // e_phoff is the program header offset 
		SectionHeader = (Elf32_Shdr *)(&ProgramData[Header->e_shoff]);      // e_shoff is the section header offset 

		/* ISR offset holds base address of the program in the physical memory */
		ISR_Offset = ( ( SectionHeader + 1 )->sh_offset );
		
		/* Calculate program size in blockes of 1000 */
		/* The ISR offset is subtracted from total memory sizes, because the first mem size holds the acual mem size pluse the program offset in the physical memory */
		/* Casting the DATA_BLOCK_SIZE as float so the division remaning is not discarded, so we can get the ceilling of the division operation */
		Program_Size = ceil( ( (ProgramHeader[0].p_memsz) + (ProgramHeader[1].p_memsz) - ISR_Offset ) / (float)DATA_BLOCK_SIZE );

		/* Writting the program data of the first file size in a buffer to be sent and flashed */
		/* subtracting the ISR offset because the first file size holds the offset plus actual file size */
		for(i= 0; i < ((ProgramHeader[0].p_filesz) - ISR_Offset ); i++ ){

			ProgramDataToSend[i] = ProgramData[ ISR_Offset + ProgramHeader[0].p_offset + i ];
			/*printf("%d",ProgramDataToSend[i]);*/
			/*fprintf(tstFileDescritor,"%d",ProgramDataToSend[i]);*/

		}
		
		for(i= 0 ; i < (ProgramHeader[1].p_filesz) ; i++ ){

			/* The ProgramDataToSend's index has +1 because the first mem size holds the last physical address of the first LOAD */
			ProgramDataToSend[  (ProgramHeader[0].p_memsz - ISR_Offset ) + 1 + i  ] = ProgramData[  ProgramHeader[1].p_offset + i   ];

		}
		
		/************************************ Erasing Sequence *******************************************/
	    /* Sending Erase Command */
	    EraseCommand.SectionsCount = Program_Size;
	    EraseCommand.SectionOffset = ISR_Offset;
	    EraseCommand.CheckSum = ( (uint8_t)EraseCommand.SectionsCount  ) + (  (uint8_t)EraseCommand.SectionOffset );
	    /* Tprotocol forms the CommandToSend by placing start byte, messageID byte, EraseCommand 5 bytes, end of frame byte */
	    TProtcol_sendFrame((void*)&EraseCommand, CommandToSend, ID_EraseCommand );
		for(txtindex=0;txtindex<8;txtindex++)
		{
			fprintf(tstFileDescritor,"%.02x",CommandToSend[txtindex]);
		}
		fprintf(tstFileDescritor ,"\n");
		/*sendCommand(CommandToSend);*/
        /************************************ Data Transmission Sequence *******************************************/
		/* loop on ProgramDataToSend with the number of number of the sections to be flashed*/
		/* loop on ProgramDataToSend with the number of number of the sections to be flashed*/
		while ( (SentDataBlockIDX < EraseCommand.SectionsCount ) ){
			/* Send 1000 Byte, 5 bytes at a time for 200 itirations */
			for ( i=0; i < (DATA_BLOCK_SIZE/FRAME_DATA_BYTES) ; i++ ){

				SentDataIDX = i*5 ;
				TProtcol_sendFrame(&ProgramDataToSend[lastSavedIDX + SentDataIDX], CommandToSend, ID_DataCommand);

				CheckSum += (ProgramDataToSend[lastSavedIDX + SentDataIDX] + ProgramDataToSend[lastSavedIDX + SentDataIDX + 1] + ProgramDataToSend[lastSavedIDX + SentDataIDX + 2] + ProgramDataToSend[lastSavedIDX + SentDataIDX + 3] + ProgramDataToSend[lastSavedIDX + SentDataIDX + 4 ]);
				/*sendCommand(CommandToSend);*/
				for(txtindex=0;txtindex<8;txtindex++)
		        {
		        	fprintf(tstFileDescritor,"%.02x",CommandToSend[txtindex]);
		        }
		        fprintf(tstFileDescritor ,"\n");
			}
			
			/*verify with checksome*/
			VerifyCommand.CheckSum = CheckSum;
			TProtcol_sendFrame( &VerifyCommand, CommandToSend, ID_VerifyCommand);
			/*sendCommand(CommandToSend);*/
			for(txtindex=0;txtindex<8;txtindex++)
		    {
				fprintf(tstFileDescritor,"%.02x",CommandToSend[txtindex]);
		    }
		    fprintf(tstFileDescritor ,"\n");
			CheckSum = 0;
		
			SentDataBlockIDX++;
			
			/* +5 because the last update to SentDataIDX will equal 995 */
			lastSavedIDX += SentDataIDX + 5; //0	1000	2000		30000
			printf("\n\n/*************************************/\n");
			printf("Last Saved IDX %ld\n",lastSavedIDX);
			printf("/*************************************/\n\n");	
			/*repeat till end*/
		}
			
		
	}
	
	fclose(tstFileDescritor);

}



