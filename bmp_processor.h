#ifndef BMP_PROCESSOR_H
#define BMP_PROCESSOR_H

#include <fstream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

#pragma pack(push, 1)
struct BitmapFileHeader {
    uint16_t signature;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;
};

struct BitmapInfoHeader {
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t importantColors;
};
#pragma pack(pop)

struct Pixel {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha;
};

class BMPProcessor {
private:
    string filename;
    BitmapFileHeader fileHeader;
    BitmapInfoHeader infoHeader;
    vector<vector<Pixel>> imageData;
    bool is32Bit;
    
    bool readHeaders(ifstream& file);
    bool readImageData(ifstream& file);
    bool writeHeaders(ofstream& file) const;
    bool writeImageData(ofstream& file) const;
    void drawLine(int x1, int y1, int x2, int y2, const Pixel& color);
    bool isValidPixel(const Pixel& pixel) const;
    char pixelToChar(const Pixel& pixel) const;

public:
    BMPProcessor();
    ~BMPProcessor();
    
    bool loadImage(const string& filename);
    bool saveImage(const string& filename) const;
    void displayImage() const;
    void drawCross();
    
    int getWidth() const { return infoHeader.width; }
    int getHeight() const { return infoHeader.height; }
    bool isLoaded() const { return !imageData.empty(); }
};

#endif 