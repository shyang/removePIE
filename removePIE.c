#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdint.h>
#include <mach-o/loader.h>

void hexify(unsigned char *data, uint32_t size){
	while(size--)
		printf("%02x", *data++);}

int main(int argc, char *argv[]){
	struct mach_header currentHeader;
	//int32_t magic = 0;
	FILE *fp; //edited file pointer

	if(argc < 1){	
		printf("Please enter the filename binary: in the format removePIE filename");
		return EXIT_FAILURE;}
	if((fp = fopen(argv[1], "rb+")) == NULL) {
		printf("Error, unable to open file\n");
		return EXIT_FAILURE; }

	
	if((fread(&currentHeader.magic, sizeof(int32_t), 1, fp)) == (int)NULL)
	{printf("Error reading magic constant in file\n");
		return EXIT_FAILURE;}
	if(currentHeader.magic == MH_MAGIC || currentHeader.magic == MH_MAGIC_64) { //little endian
		printf("loading header\n");
		fseek(fp, 0, SEEK_SET);
		if((fread(&currentHeader, sizeof(currentHeader), 1, fp)) == (int)NULL)
		{
			printf("Error reading MACH-O header");
			return EXIT_FAILURE;
		}	

		printf("\nmach_header:\t");
		hexify((unsigned char *)&currentHeader,sizeof(currentHeader));	
		printf("\noriginal flags:\t");
		hexify((unsigned char *)&currentHeader.flags, sizeof(currentHeader.flags));
		printf("\nDisabling ASLR/PIE ...\n");
		currentHeader.flags &= ~MH_PIE;
		printf("new flags:\t");
		hexify((unsigned char *)&currentHeader.flags, sizeof(currentHeader.flags));
		
		fseek(fp, 0, SEEK_SET);
		if((fwrite(&currentHeader, sizeof(char), 28, fp)) == (int)NULL)
		{
			printf("Error writing to application file %s\n",argv[1]);
		}
		printf("\nASLR has been disabled for %s\n", argv[1]);
		//exit and close memory
		//free(mach_header);
		fclose(fp);
		return EXIT_SUCCESS;	
	}
	else if(currentHeader.magic == MH_CIGAM) // big endian
	{
		printf("file is big-endian, not an iOS binary");
		return EXIT_FAILURE;
	}
	else
	{
		printf("File is not a MACH_O binary"); 
		return EXIT_FAILURE;
	}
	//exit
	return EXIT_FAILURE;	 
}
