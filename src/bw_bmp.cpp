#include "../include/bmp_functions.hpp"

void BmpApp::convertBlackWhite(FILE *f) {
    if (!bmp.fetchBmpData(f))
        return;

    for (int i = 0; i < bmp.pixels->height; i++) {
        for (int j = 0; j < bmp.pixels->width; j++) {
            const int r = bmp.pixels->data[i][j].red;
            const int g = bmp.pixels->data[i][j].green;
            const int b = bmp.pixels->data[i][j].blue;
            const int average = (r + g + b) / 3;
            bmp.pixels->data[i][j] = RGBColor(average, average, average);
        }
    }

    char *filename = (char *)malloc(100 * sizeof(char));
    sprintf(filename, "./dist/black_white.bmp");
    FILE *out = fopen(filename, "wb");
    if (!out) {
        fprintf(stderr, "Could not create file: %s\n", filename);
        free(filename); 
    }
    bmp.pixels->flatMatrix();
    bmp.writeBmpData(out, bmp);
    free(filename);
}