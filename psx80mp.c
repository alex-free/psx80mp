
#include <stdio.h>
#include <string.h>

#define BYTES_PER_SECTOR 2352
#define NUMBER_OF_DUMMY_SECTORS 27000 // 6 minutes of dummy data (CDDA silence) to be appended
#define PSX_SECTOR_MAX 319500 // 10584000 bytes per minute * 71 minutes (max officially supported for PSX CD-ROMs) = 751464000 bytes max / 2352 bytes per sector = 319500 sector max

int main(int argc, char **argv)
{
    FILE *bin;

    printf("PSX 80 Minute Patcher (PSX80MP) %s\n(c) 2023-2024, Alex Free (3-BSD)\nhttps://alex-free.github.io/psx80mp\n\n", VERSION);
    unsigned int number_of_sectors_in_bin_file;

	if(argc != 2)	
	{
	    printf("Error: incorrect number of arguments\nUsage:\n\npsx80mp <data track bin file>\n\n<data track bin file>\tThis is almost always the first track. If the game still doesn't work, try only patching the second track instead on a clean image.\n");
		return 1;
	}

	bin = fopen(argv[1],"r+b");

	if(bin == NULL) {
		printf("Error: data track bin file %s does not exist\n", argv[1]);
        return 1;
	}

    fseek(bin, 0, SEEK_END);
    number_of_sectors_in_bin_file = (ftell(bin) / BYTES_PER_SECTOR);

    if(number_of_sectors_in_bin_file > PSX_SECTOR_MAX)
    {
        printf("Error, the data track bin file: %s has too many sectors to be an unmodified PSX game disc image, is this already patched?\n", argv[1]);
        fclose(bin);
        return 1;
    }
    
    printf("Pre-patch sector count: %d\n", number_of_sectors_in_bin_file);
    char dummy_sector[BYTES_PER_SECTOR];
    memset(dummy_sector, 0, BYTES_PER_SECTOR); // Filled with zeros.

    for(int i = 0; i < NUMBER_OF_DUMMY_SECTORS; i++)
    {
       if(fwrite(dummy_sector, 1, BYTES_PER_SECTOR, bin) != BYTES_PER_SECTOR)
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