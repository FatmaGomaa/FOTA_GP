#include "elf_parser.h"

typedef struct{
	char Key[3];
}KeyStruct;

char Buffer[1024*1024]={0};
char *ProgramHeader1 = &Buffer[0x010000];
char *ProgramHeader2 = &Buffer[0x020000];
char* ElfFileName= NULL;
long size=0;
FILE * ElfFileDescriptor;

uint32_t i; 

void main(int argc, char *argv[]){
	if(argv[1] != '\0'){
		ElfFileName = argv[1];
		//printf("%s", argv[1]);
		ElfFileDescriptor = fopen(ElfFileName, "rb"); // read mode
	}

	if (ElfFileDescriptor == NULL)
	{
		printf("Error while opening the file.\n");
	}else{
		
		/*get size of Elf File*/
		fseek(ElfFileDescriptor,0,SEEK_END);
		size = ftell(ElfFileDescriptor);
		fseek(ElfFileDescriptor,0,SEEK_SET);
		rewind(ElfFileDescriptor);
		
		/*save elf file content in buffer*/
		char ch = fgetc(ElfFileDescriptor);
		long long i=0;
		while (i != size)
		{
		Buffer[i]=ch;
		i++;
		ch = fgetc(ElfFileDescriptor);
		}
		//printf("%x\n",i);
		//fgets(Buffer, size, ElfFileDescriptor);
		KeyStruct * StringTemp = &Buffer[1];

		/*close elf file to open the output file*/
		fclose(ElfFileDescriptor);
		
		//command new app
		//wait for pos resp from BL
		//8 DataBytes ProgramHeader1
		

	}
	fclose(OutputFileDescriptor);

}