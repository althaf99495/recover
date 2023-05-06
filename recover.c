#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./recover IMAGE");
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
    {
        printf("Error: unable to open file %s\n", argv[1]);
        return 1;
    }
    BYTE buffer[BLOCK_SIZE];
    int counter = 0;
    char filename[8];
    FILE *img = NULL;
    while (fread(&buffer, BLOCK_SIZE, 1, f) == 1)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (!(counter == 0))
            {
                fclose(img);
            }
            sprintf(filename, "%03i.jpg", counter);
            img = fopen(filename, "w");

            if (!img)
            {
                printf("Error: unable to create file %s\n", filename);
                return 1;
            }
            counter++;

        }
        if (!(counter == 0))
        {
            fwrite(&buffer, BLOCK_SIZE, 1, img);
        }
    }
    fclose(f);
    fclose(img);
}