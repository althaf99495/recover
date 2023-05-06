#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    // Check if a folder path was provided
    if (argc != 2) {
        printf("Usage: recover [folder]\n");
        return 1;
    }

    // Open the folder
    DIR *dir = opendir(argv[1]);
    if (dir == NULL) {
        printf("Could not open %s\n", argv[1]);
        return 2;
    }

    // Create a buffer to hold the current file's name
    char *filename = malloc(256);

    // Create a buffer to hold the current file's data
    unsigned char *buffer = malloc(512);

    // Create a counter to keep track of the number of JPEGs found
    int jpeg_count = 0;

    // Create a file pointer to hold the current JPEG file
    FILE *jpeg_file;

    // Iterate through the files in the folder
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Check if the file is a JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) {
            // Close the previous JPEG file, if one was open
            if (jpeg_count > 0) {
                fclose(jpeg_file);
            }

            // Create a new JPEG file
            sprintf(filename, "%03d.jpg", jpeg_count);
            jpeg_file = fopen(filename, "w");
            if (jpeg_file == NULL) {
                printf("Could not create %s\n", filename);
                return 3;
            }

            // Write the current file's data to the new JPEG file
            fwrite(buffer, 512, 1, jpeg_file);
            jpeg_count++;
        } else if (jpeg_count > 0) {
            // Write the current file's data to the current JPEG file
            fwrite(buffer, 512, 1, jpeg_file);
        }

        // Read the next file's data
        fread(buffer, 512, 1, entry);
    }

    // Close the last JPEG file, if one was open
    if (jpeg_count > 0) {
        fclose(jpeg_file);
    }

    // Close the folder
    closedir(dir);

    // Free the allocated memory
    free(filename);
    free(buffer);

    return 0;
}