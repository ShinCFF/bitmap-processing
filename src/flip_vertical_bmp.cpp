#include "../include/bmp_functions.hpp"

void BmpApp::flipHorizontal(FILE *f) {
    if (!bmp.fetchBmpData(f))
        return;

    for (int i = 0; i < bmp.pixels->height / 2; i++) {
    for (int j = 0; j < bmp.pixels->width; j++) {
            RGBColor temp = bmp.pixels->data[i][j];
            bmp.pixels->data[i][j] = bmp.pixels->data[bmp.pixels->height - i - 1][j];
            bmp.pixels->data[bmp.pixels->height - i - 1][j] = temp;
        }
    }

    char *filename = (char *)malloc(100 * sizeof(char));
    sprintf(filename, "./dist/flip_vertical.bmp");
    FILE *out = fopen(filename, "wb");
    bmp.pixels->flatMatrix();
    bmp.writeBmpData(out, bmp);
    bmp.pixels->~PixelArray();
    fclose(out);

    free(filename);
}