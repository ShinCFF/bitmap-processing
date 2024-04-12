#include "../include/bmp_functions.hpp"

void BmpApp::scale(FILE *f, int n) {
    if (!bmp.fetchBmpData(f))
        return;

    int newWidth = bmp.pixels->width * n;
    int newHeight = bmp.pixels->height * n;
    PixelArray *newPixels = new PixelArray(newWidth * newHeight * sizeof(RGBColor) + newWidth * newHeight, newWidth, newHeight);

    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {
            newPixels->data[i][j] = bmp.pixels->data[i / n][j / n];
        }
    }

    bmp.header.info.fileSize = sizeof(BMP_HEADER_RAW) 
                            + sizeof(BmpDib) 
                            + sizeof(BmpSignature)
                            + newPixels->size; 

    bmp.dib.pixelArraySize = newPixels->size;
    bmp.dib.imageWidth = newWidth;
    bmp.dib.imageHeight = newHeight;
    // bmp.pixels = newPixels;

    char *filename = (char *)malloc(100 * sizeof(char));
    sprintf(filename, "./dist/scale_%d.bmp", n);
    FILE *out = fopen(filename, "wb");
    if (!out) {
        fprintf(stderr, "Could not create file: %s\n", filename);
        free(filename); 
    }
    fwrite(&bmp.header.signature, sizeof(BmpSignature), 1, out);
    fwrite(&bmp.header.info, sizeof(BmpHeaderInfo), 1, out);
    fwrite(&bmp.dib, sizeof(BMP_DIB_RAW), 1, out);
    newPixels->flatMatrix();
    fwrite(newPixels->rawData, newPixels->size, 1, out);
    // bmp.pixels->flatMatrix();
    // bmp.writeBmpData(out, bmp);
    // bmp.pixels->~PixelArray();
    fclose(out);

    free(filename);
    delete newPixels;
}
