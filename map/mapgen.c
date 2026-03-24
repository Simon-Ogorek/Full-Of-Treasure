#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *file = fopen("map1.bin", "wb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    unsigned short blank = 1;
    unsigned short data = 256;
    unsigned short yDelim = 65535;
    unsigned short zDelim = 65534;

    /* 
       OOOO 
       O  O
       O  O
       OOOO
    */

    // Z = 1;

    /* 
       OOOO 
    */
    for (int j = 0; j < 150; j++)
    {
        fwrite(&data, sizeof(unsigned short),1, file);
    }
    fwrite(&yDelim, sizeof(unsigned short),1, file);

    /* 
       O  O
       O  O
    */
    for (int i = 0; i < 148; i++)
    {
        fwrite(&data, sizeof(unsigned short),1, file);
        for (int j = 0; j < 148; j++)
        {
            fwrite(&blank, sizeof(unsigned short),1, file);
        }
        fwrite(&data, sizeof(unsigned short),1, file);
        fwrite(&yDelim, sizeof(unsigned short),1, file);
    }
    /* 
       OOOO 
    */
    for (int j = 0; j < 150; j++)
    {
        fwrite(&data, sizeof(unsigned short),1, file);
    }

    fwrite(&yDelim, sizeof(unsigned short),1, file);

    fwrite(&zDelim, sizeof(unsigned short),1, file);

    /* OOOO 
       OOOO
       OOOO
       OOOO
    */
    // Z = 0;
    for (int i = 0; i < 150; i++)
    {
        for (int j = 0; j < 150; j++)
        {
            fwrite(&data, sizeof(unsigned short),1, file);
        }
        fwrite(&yDelim, sizeof(unsigned short),1, file);
    }
}