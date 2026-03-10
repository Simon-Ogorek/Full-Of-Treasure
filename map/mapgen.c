#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *file = fopen("map1.bin", "wb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    unsigned short data = 256;
    unsigned short delim = 65535;
    for (int i = 0; i < 150; i++)
    {
        for (int j = 0; j < 150; j++)
        {
            //{ "tile": "grass", "x": 0, "y": 0, "z": 0 }
            fwrite(&data, sizeof(unsigned short),1, file);
        }
        fwrite(&delim, sizeof(unsigned short),1, file);
    }
}