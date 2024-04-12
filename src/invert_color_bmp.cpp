#include "../include/bmp_functions.hpp"

void BmpApp::invertColor(FILE *f) {
    if (!bmp.fetchBmpData(f))
        return;

    for (int i = 0; i < bmp.pixels->height; i++) {
        for (int j = 0; j < bmp.pixels->width; j++) {
            bmp.pixels->data[i][j].red = 255 - bmp.pixels->data[i][j].red;
            bmp.pixels->data[i][j].green = 255 - bmp.pixels->data[i][j].green;
            bmp.pixels->data[i][j].blue = 255 - bmp.pixels->data[i][j].blue;
        }
    }

    char *filename = (char *)malloc(100 * sizeof(char));
    sprintf(filename, "./dist/invert_color.bmp");
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