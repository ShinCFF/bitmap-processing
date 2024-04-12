#ifndef BMP_FUNCTIONS_HPP
#define BMP_FUNCTIONS_HPP

#include "global.hpp"

struct RGBColor {
    uint8_t blue;
    uint8_t green;
    uint8_t red;

    RGBColor() : red(0), green(0), blue(0) {}
    RGBColor(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(b) {}

    void print();
};

uint32_t getPadding(uint32_t w);
uint32_t getPixelArraySize(uint32_t w, uint32_t h);

struct PixelArray {
    uint32_t size;
    uint32_t width, height;
    uint32_t padding;
    uint8_t *rawData = NULL;
    RGBColor **data = NULL;

    PixelArray();
    PixelArray(uint32_t size, uint32_t w, uint32_t h);
    ~PixelArray();

    void print();
    void toMatrix();
    void flatMatrix();
};

struct BmpSignature {
    uint8_t data[2];
    BmpSignature();
};

struct BmpHeaderInfo {
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t pixelArrayOffsetBytes;
};

struct BMP_HEADER_RAW {
    BmpSignature signature;
    BmpHeaderInfo info;
};

struct BmpHeader : BMP_HEADER_RAW {
    void read(FILE *f);
    void print();
};

struct BMP_DIB_RAW {
    uint32_t dibSize;
    uint32_t imageWidth;
    uint32_t imageHeight;
    uint16_t colorPlaneCount;
    uint16_t colorDepth;
    uint32_t compressionMethod;
    uint32_t pixelArraySize;
    uint32_t horizontalResolution;
    uint32_t verticalResolution;
    uint32_t colorCount;
    uint32_t importantColorCount;
};

struct BmpDib : BMP_DIB_RAW {
    void read(FILE *f);
    void print();
};

struct BmpFile {
    BmpHeader header;
    BmpDib dib;
    PixelArray *pixels = nullptr;
    FILE *openFile(const char *filename);
    bool is24Bit();
    bool isBmp();
    void readFileInfo(FILE *f);
    void readPixels(FILE *f);
    bool fetchBmpData(FILE *f);
    void writeBmpData(FILE *f, BmpFile &bmp);
    void printInfo(bool printPixels);
};

struct BmpApp {
    BmpFile bmp;

    bool isValidArg(int argc);
    void cutBitmap(FILE *f, int hParts, int wParts);
    void convertBlackWhite(FILE *f);

    void flipHorizontal(FILE *f);
    void flipVertical(FILE *f);

    void scale(FILE *f, int n);
    void downResolution(FILE *f, int intensity);

    void invertColor(FILE *f);
    void grayScale(FILE *f);
};

#endif