#include "../include/bmp_functions.hpp"

void BmpApp::flipVertical(FILE *f) {
    if (!bmp.fetchBmpData(f))
        return;

    for (int i = 0; i < bmp.pixels->height; i++) {
    for (int j = 0; j < bmp.pixels->width / 2; j++) {
            RGBColor temp = bmp.pixels->data[i][j];
            bmp.pixels->data[i][j] = bmp.pixels->data[i][bmp.pixels->width - j - 1];
            bmp.pixels->data[i][bmp.pixels->width - j - 1] = temp;
        }
    }

    char *filename = (char *)malloc(100 * sizeof(char));
    sprintf(filename, "./dist/flip_horizontal.bmp");
    FILE *out = fopen(filename, "wb");

    bmp.pixels->flatMatrix();
    bmp.writeBmpData(out, bmp);
    bmp.pixels->~PixelArray();
    fclose(out);

    free(filename);
}