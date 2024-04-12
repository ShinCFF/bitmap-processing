#include "../include/bmp_functions.hpp"

void RGBColor::print() {
    printf("RGB(%u, %u, %u)\n", (unsigned int)red, (unsigned int)green, (unsigned int)blue);
}

uint32_t getPadding(uint32_t w) {
    return (4 - (w * sizeof(RGBColor)) % 4) % 4;
}

uint32_t getPixelArraySize(uint32_t w, uint32_t h) {
    return h * w * sizeof(RGBColor) + getPadding(w) * h;
}

PixelArray::PixelArray() : size(0), width(0), height(0), padding(0) {
    rawData = NULL;
    data = NULL;
}

PixelArray::PixelArray(uint32_t size, uint32_t w, uint32_t h)
    : size(size), width(w), height(h), padding(getPadding(w)) {
    rawData = NULL;

    data = (RGBColor **)malloc(h * sizeof(RGBColor *));

    for (int i = 0; i < h; i++)
        data[i] = (RGBColor *)malloc(w * sizeof(RGBColor));
}

PixelArray::~PixelArray() {
    free(rawData);
    for (int i = 0; i < height; i++)
        free(data[i]);
    free(data);
}

void PixelArray::print() {
    printf("==== PIXEL ARRAY INFO ====\n");
    printf("+ Size  : %d byte(s)\n", size);
    printf("+ Width : %d pixel(s)\n", width);
    printf("+ Height: %d pixel(s)\n", height);
    printf("+ Pixel Array:\n");
    for (int i = 0; i < size; i++) {
        printf("%u ", (unsigned int)rawData[i]);
    }
    printf("\n");
    for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++) {
        data[i][j].print();
    }
}

void PixelArray::toMatrix() {
    for (int rowIdx = 0; rowIdx < height; rowIdx++) {
    for (int colIdx = 0; colIdx < width * sizeof(RGBColor) + padding;) {
        int idx = colIdx + rowIdx * (width * sizeof(RGBColor) + padding);
        if (colIdx < width * sizeof(RGBColor)){
            uint8_t _b = rawData[idx];
            uint8_t _g = rawData[idx + 1];
            uint8_t _r = rawData[idx + 2];
            RGBColor color(_r, _g, _b);
            data[rowIdx][colIdx / sizeof(RGBColor)] = color;
            colIdx += sizeof(RGBColor);
        } else
            colIdx += padding;
        }
    }
}

void PixelArray::flatMatrix() {
    free(rawData);
    rawData = (uint8_t *)malloc(size * sizeof(uint8_t));
    int rawDataIndex = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            rawData[rawDataIndex] = data[i][j].blue;
            rawData[rawDataIndex + 1] = data[i][j].green;
            rawData[rawDataIndex + 2] = data[i][j].red;
            rawDataIndex += 3;
        }
        for (int j = 0; j < padding; j++) {
            rawData[rawDataIndex] = 0;
            rawDataIndex++;
        }
    }
}

BmpSignature::BmpSignature() { 
    data[0] = data[1] = 0; 
}

void BmpHeader::read(FILE *f) {
    if (!f) return;
    fseek(f, 0, SEEK_SET);
    fread(&signature, sizeof(BmpSignature), 1, f);
    fread(&info, sizeof(BmpHeaderInfo), 1, f);
}

void BmpHeader::print() {
    printf("==== BMP HEADER ====\n");
    printf("+ Signature  : %c%c\n", signature.data[0], signature.data[1]);
    printf("+ File Size  : %d byte(s)\n", info.fileSize);
    printf("+ Reserved1  : %d\n", info.reserved1);
    printf("+ Reserved2  : %d\n", info.reserved2);
    printf("+ Data Offset: %d byte(s)\n", info.pixelArrayOffsetBytes);
}

void BmpDib::read(FILE *f) {
    if (!f) return;
    fseek(f, sizeof(BmpSignature) + sizeof(BmpHeaderInfo), SEEK_SET);
    fread(this, sizeof(BmpDib), 1, f);
}

void BmpDib::print() {
    printf("==== BMP DIB ====\n");
    printf("+ DIB Size        : %d\n", dibSize);
    printf("+ Img Width       : %d pixel(s)\n", imageWidth);
    printf("+ Img Height      : %d pixel(s)\n", imageHeight);
    printf("+ Color Planes    : %d\n", colorCount);
    printf("+ Color Depth     : %d bit(s)\n", colorDepth);
    printf("+ Compression     : %s\n", COMPRESSION_METHOD[compressionMethod]);
    printf("+ Pixel Array Size: %d byte(s)\n", pixelArraySize);
    printf("+ X Pixels/m      : %d\n", horizontalResolution);
    printf("+ Y Pixels/m      : %d\n", verticalResolution);
}

FILE *BmpFile::openFile(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "Could not open file: %s\n", filename);
        return NULL;
    }
    return f;
}

bool BmpFile::is24Bit() { 
    return dib.colorDepth == 24; 
}

bool BmpFile::isBmp() {
    return header.signature.data[0] == 'B' && header.signature.data[1] == 'M';
}

void BmpFile::readFileInfo(FILE *f) {
    header.read(f);
    dib.read(f);
}

void BmpFile::readPixels(FILE *f) {
    pixels = new PixelArray(dib.pixelArraySize, dib.imageWidth, dib.imageHeight);
    uint8_t *buf = (uint8_t *)malloc(pixels->size * sizeof(uint8_t));
    fseek(f, header.info.pixelArrayOffsetBytes, SEEK_SET);
    fread(buf, sizeof(uint8_t) * pixels->size, 1, f);
    pixels->rawData = buf;  
    pixels->toMatrix();
}

bool BmpFile::fetchBmpData(FILE *f) {
    if (!f) return false;
    readFileInfo(f);
    if (!isBmp() || !is24Bit()) {
        fprintf(stderr, "Invalid BMP 24-bit file\n");
        return false;
    }
    readPixels(f);
    return true;
}

void BmpFile::writeBmpData(FILE *f, BmpFile &bmp) {
    if (!f) return; 
    fwrite(&bmp.header.signature, sizeof(BmpSignature), 1, f);
    fwrite(&bmp.header.info, sizeof(BmpHeaderInfo), 1, f);
    fwrite(&bmp.dib, sizeof(BMP_DIB_RAW), 1, f);
    fwrite(bmp.pixels->rawData, bmp.pixels->size, 1, f);
}

void BmpFile::printInfo(bool printPixels = false) {
    header.print();
    dib.print();
    if (printPixels)
        pixels->print();
}

bool BmpApp::isValidArg(int argc) { 
    return !(argc == 4 || argc == 6); 
}
