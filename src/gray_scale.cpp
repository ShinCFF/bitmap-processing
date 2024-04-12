#include "../include/bmp_functions.hpp"

void BmpApp::grayScale(FILE *f) {
    if (!bmp.fetchBmpData(f))
        return;

    convertBlackWhite(f);

    for (int i = 0; i < bmp.pixels->height; i++) {
        for (int j = 0; j < bmp.pixels->width; j++) {
            if (bmp.pixels->data[i][j].red > 80)
                continue;
            bmp.pixels->data[i][j].red = 0;
            bmp.pixels->data[i][j].green = 0;
            bmp.pixels->data[i][j].blue = 0;
        }
    }

    char *filename = (char *)malloc(100 * sizeof(char));
    sprintf(filename, "./dist/output.grayScaleTozero.bmp");
    FILE *out = fopen(filename, "wb");
    if (!out) {
        fprintf(stderr, "Could not create file: %s\n", filename);
        free(filename); 
    }
    bmp.pixels->flatMatrix();
    bmp.writeBmpData(out, bmp);
    fclose(out);
    free(filename);
}