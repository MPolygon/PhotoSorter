#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    


    FILE *fp;
    int c, i, max;
    int header[8];

    int png_header[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    int jpeg_header[2] = {255, 216};

    // File: /mnt/d/Blender/Textures/bark_brown_02_2k_jpg/bark_brown_02_diff_2k.jpg
    // File: /mnt/d/Blender/Textures/drive-download-20210427T201332Z-001/Lace_001_height.png

    fp = fopen("/mnt/d/Blender/Textures/drive-download-20210427T201332Z-001/Lace_001_height.png", "rb");
    if (fp == NULL) {
        fprintf(stderr, "cannot open input file\n");
        return 1;
    }
    for (i = 0, max = 8; i < max && (c = getc(fp)) != EOF; i++) {
        header[i] = c;
        printf("%02x/%d\n", c, header[i]);
        // printf("%d", c);
        /* if (i % 16 == 15)
            putchar('\n');  // 16 bytes per line
        // else if (i % 2 == 1)
        else {
            putchar(' ');   // group bytes in pairs
        } */
    }
    if (i % 16 != 0)
        putchar('\n');  // output a newline if needed

    fclose(fp);

    return 0;
}