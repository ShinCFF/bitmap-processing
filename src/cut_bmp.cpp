#include "../include/bmp_functions.hpp"

void BmpApp::cutBitmap(FILE *f, int heightParts, int widthParts) {
    if (!bmp.fetchBmpData(f))
        return;
    
    int heightPartsize = bmp.pixels->height / heightParts;
    int widthPartsize = bmp.pixels->width / widthParts;
    int hPartRemainder = bmp.pixels->height % heightParts;
    int wPartRemainder = bmp.pixels->width % widthParts;
    for (int i = 0; i < heightParts; i++) {
        for (int j = 0; j < widthParts; j++) {
            char *filename = (char *)malloc(100 * sizeof(char));
            sprintf(filename, "./dist/part-%d-%d.bmp", i, j);

            FILE *out = fopen(filename, "wb");
            if (!out) {
                fprintf(stderr, "Could not create file: %s\n", filename);
                free(filename); 
                break;
            }

            int thisPartWidth = widthPartsize + (j == widthParts - 1 ? wPartRemainder : 0);
            int thisPartHeight = heightPartsize + (i == 0 ? hPartRemainder : 0);

            BmpFile partBMP = bmp;
            partBMP.pixels = new PixelArray(getPixelArraySize(thisPartWidth, thisPartHeight),thisPartWidth, thisPartHeight);
            partBMP.header.info.fileSize = sizeof(BmpSignature) 
                                        + sizeof(BMP_HEADER_RAW) 
                                        + sizeof(BMP_DIB_RAW) 
                                        + partBMP.pixels->size;

            partBMP.dib.pixelArraySize = partBMP.pixels->size;
            partBMP.dib.imageWidth = thisPartWidth;
            partBMP.dib.imageHeight = thisPartHeight;

            for (int k = 0; k < thisPartHeight; k++)
            for (int l = 0; l < thisPartWidth; l++)
                partBMP.pixels->data[k][l] = bmp.pixels->data[i * heightPartsize + k][j * widthPartsize + l];

            partBMP.pixels->flatMatrix();
            partBMP.writeBmpData(out, partBMP);
            fclose(out);

            free(filename);
        }
    }
}