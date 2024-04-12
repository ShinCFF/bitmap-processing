#include "../include/bmp_functions.hpp"

int main(int argc, char *argv[]) {
    BmpApp app;
    FILE *f = app.bmp.openFile(argv[1]);
    if (!f)
        return 1;

    if (app.isValidArg(argc)) {
        fprintf(stderr, "Unsuitable arguments!\n");
        fprintf(stderr, "Usage: %s <filename> -h <heightParts> -w <widthParts>\n", "cutbmp");
        fprintf(stderr, "Example: %s ./img.bmp -h 2\n", "cutbmp");
        fprintf(stderr, "Example: %s ./img.bmp -h 2 -w 2\n", "cutbmp");
        return 1;
    }

    int heightParts = 1, widthParts = 1;
    for (char c = 0; (c = getopt(argc, argv, "-h:-w:")) != -1;) {
        if (!optarg)
            continue;
        switch (c) {
        case 'h':
            heightParts = atoi(optarg);
            break;
        case 'w':
            widthParts = atoi(optarg);
            break;
        }
    }

    app.bmp.fetchBmpData(f);
    app.bmp.printInfo(false);
    
    app.cutBitmap(f, heightParts, widthParts);
    app.convertBlackWhite(f);

    app.flipHorizontal(f);
    app.flipVertical(f);
    app.invertColor(f);

    app.scale(f, 2);
    app.downResolution(f, 8);
    // app.grayScale(f);

    fclose(f);
    return 0;
}
