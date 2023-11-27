
#include <stdio.h>

#define VERSION "2.0"
#define BYTES_PER_SECTOR 2352
#define NUMBER_OF_DUMMY_SECTORS 27000
// 10584000 bytes per minute * 71 minutes (max officially supported for PSX CD-ROMs) = 751464000 bytes max / 2352 bytes per sector = 319500 sector max
#define PSX_SECTOR_MAX 319500

int main(int argc, char **argv)
{
    FILE *bin;

    printf("PSX80MP Patcher v%s By Alex Free (C)2023 (3-BSD)\nhttps://alex-free.github.io/psx80mp\n\n", VERSION);
    unsigned int number_of_sectors_in_bin_file;

	if(argc != 2)	
	{
	    printf("Error: incorrect number of arguments\nUsage:\n%s <track01.bin>\n<track01.bin>		first data track of a PSX game\n", argv[0]);
		return 1;
	}

	bin = fopen(argv[1],"r+b");
	if(bin == NULL) {
		printf("Error: bin file %s does not exist\n", argv[1]);
        return 1;
	}

    fseek(bin, 0, SEEK_END);
    number_of_sectors_in_bin_file = (ftell(bin) / BYTES_PER_SECTOR);
    if(number_of_sectors_in_bin_file > PSX_SECTOR_MAX)
    {
        printf("Error, the bin file: %s has too many sectors to be an unmodified PSX game disc image, is this already patched?\n", argv[1]);
        fclose(bin);
        return 1;
    }
    
    printf("Pre-patch sector count: %d\n", number_of_sectors_in_bin_file);
    
    for(int i = 0; i < NUMBER_OF_DUMMY_SECTORS; i++)
    {
       if(fwrite("0", 1, BYTES_PER_SECTOR, bin) != BYTES_PER_SECTOR)
       {
            printf("Error writing dummy sector %d\n", i);
            fclose(bin);
            return 1;
       }
    }

    fseek(bin, 0, SEEK_END);
    number_of_sectors_in_bin_file = (ftell(bin) / BYTES_PER_SECTOR);
    fclose(bin);
    printf("Post-patch sector count: %d\nComplete, ready to burn to an 80 Minute CD-R\n", number_of_sectors_in_bin_file);
}