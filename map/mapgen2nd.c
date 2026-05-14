#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *file = fopen("map0.bin", "wb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    unsigned short blank = 65533;
    unsigned short rock = 3;
    unsigned short water = 2;

    unsigned short yDelim = 65535;
    unsigned short zDelim = 65534;
    unsigned short end = 0;

    // Z = 0;
    for (int i = 0; i < 150; i++)
    {
        for (int j = 0; j < 150; j++)
        {
            if (i > 100 && j - (i/5) < 50)
                fwrite(&water, sizeof(unsigned short),1, file);
            else
                fwrite(&rock, sizeof(unsigned short),1, file);
        }
        fwrite(&yDelim, sizeof(unsigned short),1, file);
    }

    /* 
       OOOO 
       O  O
       O  O
       OOOO
    */  
    fwrite(&zDelim, sizeof(unsigned short), 1, file);
    // Z = 1;

    /* 
       OOOO 
    */
    for (int j = 0; j < 150; j++)
    {
        fwrite(&rock, sizeof(unsigned short),1, file);
    }
    fwrite(&yDelim, sizeof(unsigned short),1, file);

    /* 
       O  O
       O  O
    */
    for (int i = 0; i < 148; i++)
    {
        fwrite(&rock, sizeof(unsigned short),1, file);
        for (int j = 0; j < 148; j++)
        {
            fwrite(&blank, sizeof(unsigned short),1, file);
        }
        fwrite(&rock, sizeof(unsigned short),1, file);
        fwrite(&yDelim, sizeof(unsigned short),1, file);
    }
    /* 
       OOOO 
    */
    for (int j = 0; j < 150; j++)
    {
        fwrite(&rock, sizeof(unsigned short),1, file);
    }

    fwrite(&yDelim, sizeof(unsigned short),1, file);

    fwrite(&zDelim, sizeof(unsigned short),1, file);

    /* OOOO 
       OOOO
       OOOO
       OOOO
    */
    
    fwrite(&end, sizeof(unsigned short),1, file);
}