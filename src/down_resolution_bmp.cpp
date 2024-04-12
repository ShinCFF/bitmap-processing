#include "../include/bmp_functions.hpp"

void BmpApp::downResolution(FILE *f, int intensity) {
    if (!bmp.fetchBmpData(f))
        return;

    for (int i = 0; i < bmp.pixels->height; i++) {
        for (int j = 0; j < bmp.pixels->width; j++) {
            bmp.pixels->data[i][j] = bmp.pixels->data[i - i % intensity][j - j % intensity];
        }
    }

    char *filename = (char *)malloc(100 * sizeof(char));
    sprintf(filename, "./dist/censor.bmp");
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