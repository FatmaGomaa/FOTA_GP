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

HANDLE hComm;                                           // Handle to the Serial port
unsigned char * ComPortName;                            // Name of the Serial port passed from GUI
BOOL  Status;                                           // Status of the various operations 
DWORD dwEventMask;                                      // Event mask to trigger from type DWORD (unsigned long)
unsigned char  TempChar;                                // Temperory Character
unsigned char  SerialBuffer[256];                       // Buffer Containing Rxed Data
unsigned char * lpBuffer;		       		            // Buffer used for data written on the port  
DWORD  dNoOFBytestoWrite;                               // No of bytes to write into the port
DWORD  dNoOfBytesWritten = 0;                           // No of bytes written to the port
DWORD NoBytesRead;                                      // Bytes read by ReadFile()
unsigned char CommandToSend[9] = {0};			        // Buffer holding the Frame to be sent or received (Frame Size + Null Terminator)
unsigned long long z = 0,x = 0;						    // Iterators used for producing delay
long long i = 0;                                        // Iterators used in looping 
unsigned int m = 0;                                     // Iterators used in looping


unsigned char ProgramData[1024*1024] = {0};		        // 60 K Program Data Buffer, will contain the whole elf file
unsigned char ProgramDataToSend[30*1024] = {0};	        // 30 K only Program Header Sections, will contain the data of the program 
unsigned char * ElfFileName;                            // Pointer to hold the elf file name passed from the GUI
unsigned char * MCU_Type;                               // Pointer to hold the MCU type passed from the GUI
FILE * ElfFileDescriptor;                               // Discriptor used to deal with the elf file
FILE * ProgressFileDescriptor;                          // Discriptor used to deal with the progress file
unsigned char ProgressFileName[] = "F:\\Tourky\\FOTA_GP\\BootLoader\\GUI\\progress.txt";      // Progress file set as interface between CommReceiver and the GUI progress bar
unsigned long size = 0;                                 // Elf file size
unsigned static long SentDataIDX = 0;                   // Index of the data in ProgramDataToSend buffer
unsigned static long SentDataBlockIDX = 0;              // Index to the number of data blocks sent to the BL to be flashed
unsigned static long CheckSum = 0;                      // Checksum for Erase frame and for the verify Frame
unsigned char MessageID = 0;                            // Eash frame will have a different Message ID
unsigned long lastSavedIDX = 0;                         // Index for last send (saved) data 
unsigned long ISR_Offset = 0;                           // ISR offset holds base address of the program in the physical memory
unsigned char Program_Size = 0;                         // Program size in flash in sections (blockes)

Elf32_Ehdr * Header = NULL;                             // Pointer to the elf file header
Elf32_Phdr * ProgramHeader = NULL;                      // Pointe to the program header in the elf file 
Elf32_Shdr * SectionHeader = NULL;                      // Pointe to the section header in the elf file 


void main(int argc, char *argv[])
{
	
	/* Extracting McU type, COM Number and elf File Name  from passed input arguments */
	MCU_Type = argv[1];
	ComPortName = argv[2];
	ElfFileName = argv[3];	

	/* Initialization of used Commands in Code */	
	EraseCommand_t EraseCommand = {0};
	ResponseCommand_t ResponseCommand = {0};
	DataCommand_t DataCommand = {0};
	VerifyCommand_t VerifyCommand = {0};

	/* open the elf file with read stream of bytes permission */
	ElfFileDescriptor = fopen(ElfFileName, "rb"); 
	
    /* Initialize the buffer that will hold the program with 0xFF, so if (mem_size - file_size > 0) the extra bytes will be 0xFF */
	long long ProgramIterator = 0;
	for(ProgramIterator = 0;ProgramIterator < sizeof(ProgramDataToSend); ProgramIterator++){
		ProgramDataToSend[ProgramIterator] = 0xFF;
	}
  
	/* Printing welcoming sentence to indecate the begining of the flashing */
	InitSerialPort();
	printf("-				Welcome To MCU Port					\n");
	
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
		while (i != size)
		{
			ProgramData[i]=ch;
			i++;
			ch = fgetc(ElfFileDescriptor);
		}

		/* Close elf file to open the output file */
		fclose(ElfFileDescriptor);
		
		/* open the progress file with write permission */
		ProgressFileDescriptor = fopen(ProgressFileName, "w+"); // read mode
		
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

		}

		/* Writting the program data of the second file size in a buffer to be sent and flashed */
		for(i= 0 ; i < (ProgramHeader[1].p_filesz) ; i++ ){

			/* The ProgramDataToSend's index has +1 because the first mem size holds the last physical address of the first LOAD */
			ProgramDataToSend[  (ProgramHeader[0].p_memsz - ISR_Offset ) + 1 + i  ] = ProgramData[  ProgramHeader[1].p_offset + i   ];

		}
		
		
	/************************************ Erasing Sequence *******************************************/
	/* Sending Erase Command */
	openPort();
	EraseCommand.SectionsCount = Program_Size;
	EraseCommand.SectionOffset = ISR_Offset;
	EraseCommand.CheckSum = ( (uint8_t)EraseCommand.SectionsCount  ) + (  (uint8_t)EraseCommand.SectionOffset );
	/* Tprotocol forms the CommandToSend by placing start byte, messageID byte, EraseCommand 5 bytes, end of frame byte */
	TProtcol_sendFrame((void*)&EraseCommand, CommandToSend, ID_EraseCommand );
	sendCommand(CommandToSend);
	closePort();
	
	/******* waiting for response on the sent Erase Command *********/
	openPort();
	receiveCommand();
	TProtocol_ReceiveFrame( SerialBuffer, &ResponseCommand, &MessageID);
	printf("\nThe received Response is %d", MessageID);
	printf("\nThe received Response is %d\n\n", ResponseCommand.Response);
	closePort();
	rewind(ProgressFileDescriptor);

	/* Update the Progress file after eraseing */
	fprintf(ProgressFileDescriptor, "%d\t", Program_Size);	
	fprintf(ProgressFileDescriptor, "%d", SentDataBlockIDX);
	
	/************************************ Data Transmission Sequence *******************************************/
		/* loop on ProgramDataToSend with the number of number of the sections to be flashed*/
		while ( (SentDataBlockIDX < EraseCommand.SectionsCount ) ){

			/* Send 1000 Byte, 5 bytes at a time for 200 itirations */
			for ( i=0; i < (DATA_BLOCK_SIZE/FRAME_DATA_BYTES) ; i++ ){

				SentDataIDX = i*5 ;
				TProtcol_sendFrame(&ProgramDataToSend[lastSavedIDX + SentDataIDX], CommandToSend, ID_DataCommand);

				CheckSum += (ProgramDataToSend[lastSavedIDX + SentDataIDX] + ProgramDataToSend[lastSavedIDX + SentDataIDX + 1] + ProgramDataToSend[lastSavedIDX + SentDataIDX + 2] + ProgramDataToSend[lastSavedIDX + SentDataIDX + 3] + ProgramDataToSend[lastSavedIDX + SentDataIDX + 4 ]);
				openPort();
				sendCommand(CommandToSend);
				closePort();
			}
			
			/*verify with checksome*/
			VerifyCommand.CheckSum = CheckSum;
			TProtcol_sendFrame( &VerifyCommand, CommandToSend, ID_VerifyCommand);
			openPort();
			sendCommand(CommandToSend);
			closePort();
			CheckSum = 0;
			
			/*wait for response*/
			openPort();
			receiveCommand();
			closePort();
			TProtocol_ReceiveFrame( SerialBuffer, &ResponseCommand, &MessageID);
			
			/* Check on response if it is OK  increment indxs and write zero to Checksum local var. */
			if(ResponseCommand.Response == R_OK){
				SentDataBlockIDX++;
				
				/* +5 because the last update to SentDataIDX will equal 995 */
				lastSavedIDX += SentDataIDX + 5; //0	1000	2000		30000
				printf("\n\n/*************************************/\n");
				printf("Last Saved IDX %ld\n",lastSavedIDX);
				printf("/*************************************/\n\n");
				rewind(ProgressFileDescriptor);
				/* Update the Progress file after eraseing */
				fprintf(ProgressFileDescriptor, "%d\t", Program_Size);	
				/* SentDataBlockIDX + 1 , because SentDataBlockIDX starts with 0 */
				fprintf(ProgressFileDescriptor, "%d", SentDataBlockIDX + 1);
				
				if(SentDataBlockIDX == 8){
					break;
				}
			}
			else{
				
				/* Do nothing to the indxs and resend the same 1000 Data block */
			}
			
			/*repeat till end*/
		}
	}

	fclose(ProgressFileName);

}


/* A function to print on the console window as an indecation of the start of the flashing */
void InitSerialPort(void){

	printf("\n\n +==========================================+");
	printf("\n |    Serial Port  Reception (Win32 API)    |");
	printf("\n +==========================================+\n");

}

/* Setting Timeouts */
void SetTimeOut(void){
	
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout         = 50;
	timeouts.ReadTotalTimeoutConstant    = 50;
	timeouts.ReadTotalTimeoutMultiplier  = 10;
	timeouts.WriteTotalTimeoutConstant   = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (SetCommTimeouts(hComm, &timeouts) == FALSE)
		printf("\n\n    Error! in Setting Time Outs");
	else
		printf("\n\n    Setting Serial Port Timeouts Successfull");

}

/* Writing a Character to Serial Port */
void sendCommand(char * command){
	
	/* lpBuffer should be  char or byte array, otherwise write wil fail */
	lpBuffer = command;
	/* No of bytes written to the port */
	dNoOfBytesWritten = 0;                  

	/* Calculating the no of bytes to write into the port */
	dNoOFBytestoWrite = sizeof(CommandToSend); 

	Status = WriteFile(hComm,               // Handle to the Serialport
			lpBuffer,                       // Data to be written to the port 
			dNoOFBytestoWrite,              // No of bytes to write into the port
			&dNoOfBytesWritten,             // No of bytes written to the port
			NULL);

	if (Status == TRUE)
		printf("\n\n    %s - Written to %s with size %d\n", lpBuffer, ComPortName,dNoOfBytesWritten);
	else
		printf("\n\n   Error %d in Writing to Serial Port",GetLastError());

	/* Delay after sending each frame to guarantee synchronisation between this PC App and the BL project written on MCU */
	for (z=0;z<100000000;z++){
		x++;
	}

}

/* Setting Receive Mask */
void receiveCommand(void){
	
	m=0;
	
	/* Configure Windows to Monitor the serial device for Character Reception */
	Status = SetCommMask(hComm, EV_RXCHAR); 

	if (Status == FALSE)
		printf("\n\n    Error! in Setting CommMask");
	else
		printf("\n\n    Setting CommMask successfull");

	printf("\n\n    Waiting for Data Reception");

	/* Setting WaitComm() Event */
	Status = WaitCommEvent(hComm, &dwEventMask, NULL); //Wait for the character to be received

	/* Program will Wait here till a Character is received */				

	if (Status == FALSE){
		printf("\n    Error! in Setting WaitCommEvent()");
	}
	/* If  WaitCommEvent() == True, Read the RXed data using ReadFile(); */
	else {
		printf("\n\n    Characters Received");
		do
		{
			Status = ReadFile(hComm, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);
			SerialBuffer[m] = TempChar; 
			
			/* Check if the first received char is 'A' */
			if(m==0 && TempChar == 'A'){
				/*Do Nothing*/
			}
			/* If it is not 'A' then we will hardcoded ourselvs */
			else if(m==0 && TempChar != 'A'){
				SerialBuffer[0] = 'A';
				SerialBuffer[1] = TempChar;
				/*because we started from 0 and saved 2 elements*/
				m++;
			}
			m++;
			printf("\nNoBytesRead is : %d and received Data is %c \n", NoBytesRead, TempChar);
		}
		while (NoBytesRead > 0);



		/* Printing the RXed String to Console */
		printf("\n\n    ");
		int j =0;
		/*  j < i-1 to remove the dupliated last character */
		for (j = 0; j < m -1; j++)		
			printf("%c", SerialBuffer[j]);

	}	
	
	/* Delay after receiving each frame to guarantee synchronisation between this PC App and the BL project written on MCU */
	z=0;x=0;
	for (z=0;z<100000000;z++){
		x++;
	}
	x=0;
	for (z=0;z<100000000;z++){
		x++;
	}
	
}

/* Opening the Serial Port */
void openPort(void){
	
	hComm = CreateFile( ComPortName,                  // Name of the Port to be Opened
			GENERIC_READ | GENERIC_WRITE,             // Read/Write Access
			0,                                        // No Sharing, ports cant be shared
			NULL,                                     // No Security
			OPEN_EXISTING,                            // Open existing port only
			0,                                        // Non Overlapped I/O
			NULL);                                    // Null for Comm Devices

	if (hComm == INVALID_HANDLE_VALUE)
		printf("\n    Error! - Port %s can't be opened\n", ComPortName);
	else
		printf("\n    Port %s Opened\n ", ComPortName);

	/* Setting the Parameters for the SerialPort */
	/* Initializing DCB structure that Defines the control setting for a serial communications device */
	DCB dcbSerialParams = { 0 };                         
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	/* Retreives  the current settings */
	Status = GetCommState(hComm, &dcbSerialParams);      

	if (Status == FALSE)
		printf("\n    Error! in GetCommState()");

	dcbSerialParams.BaudRate = CBR_9600;      // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
	dcbSerialParams.Parity   = NOPARITY;      // Setting Parity = None 
	/* Configuring the port according to settings in DCB */
	Status = SetCommState(hComm, &dcbSerialParams);   

	if (Status == FALSE){
		 
		printf("\n    Error! in Setting DCB Structure");
	}
	
	SetTimeOut();
}

/* Function to close the Serial Port */
void closePort(void){
	CloseHandle(hComm);
}