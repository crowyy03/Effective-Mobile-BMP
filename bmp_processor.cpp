#include "bmp_processor.h"
#include <iostream>
#include <algorithm>
#include <cmath>

BMPProcessor::BMPProcessor() : is32Bit(false) {
}

BMPProcessor::~BMPProcessor() {
}

bool BMPProcessor::loadImage(const string& filename) {
    this->filename = filename;
    ifstream file(filename, ios::binary);
    
    if (!file.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл " << filename << endl;
        return false;
    }
    
    if (!readHeaders(file)) {
        cerr << "Ошибка: Неверный формат BMP файла" << endl;
        return false;
    }
    
    if (!readImageData(file)) {
        cerr << "Ошибка: Не удалось прочитать данные изображения" << endl;
        return false;
    }
    
    file.close();
    return true;
}

bool BMPProcessor::readHeaders(ifstream& file) {
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BitmapFileHeader));
    
    if (fileHeader.signature != 0x4D42) {
        return false;
    }
    
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BitmapInfoHeader));
    
    if (infoHeader.bitsPerPixel != 24 && infoHeader.bitsPerPixel != 32) {
        cerr << "Ошибка: Поддерживаются только 24 и 32 битные изображения" << endl;
        return false;
    }
    
    is32Bit = (infoHeader.bitsPerPixel == 32);
    
    if (infoHeader.compression != 0) {
        cerr << "Ошибка: Сжатые изображения не поддерживаются" << endl;
        return false;
    }
    
    return true;
}

bool BMPProcessor::readImageData(ifstream& file) {
    file.seekg(fileHeader.dataOffset);
    
    imageData.resize(infoHeader.height, vector<Pixel>(infoHeader.width));
    
    int bytesPerRow = infoHeader.width * (is32Bit ? 4 : 3);
    int padding = (4 - (bytesPerRow % 4)) % 4;
    
    for (int y = infoHeader.height - 1; y >= 0; --y) {
        for (int x = 0; x < infoHeader.width; ++x) {
            Pixel& pixel = imageData[y][x];
            
            if (is32Bit) {
                file.read(reinterpret_cast<char*>(&pixel), 4);
            } else {
                file.read(reinterpret_cast<char*>(&pixel), 3);
                pixel.alpha = 255;
            }
        }
        
        if (!is32Bit && padding > 0) {
            file.seekg(padding, ios::cur);
        }
    }
    
    return true;
}

bool BMPProcessor::saveImage(const string& filename) const {
    if (!isLoaded()) {
        cerr << "Ошибка: Нет загруженного изображения" << endl;
        return false;
    }
    
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Ошибка: Не удалось создать файл " << filename << endl;
        return false;
    }
    
    if (!writeHeaders(file)) {
        cerr << "Ошибка: Не удалось записать заголовки" << endl;
        return false;
    }
    
    if (!writeImageData(file)) {
        cerr << "Ошибка: Не удалось записать данные изображения" << endl;
        return false;
    }
    
    file.close();
    return true;
}

bool BMPProcessor::writeHeaders(ofstream& file) const {
    BitmapFileHeader writeFileHeader = fileHeader;
    BitmapInfoHeader writeInfoHeader = infoHeader;
    
    int bytesPerRow = infoHeader.width * (is32Bit ? 4 : 3);
    int padding = (4 - (bytesPerRow % 4)) % 4;
    int rowSize = bytesPerRow + padding;
    
    writeFileHeader.fileSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + 
                              rowSize * infoHeader.height;
    writeFileHeader.dataOffset = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
    
    writeInfoHeader.imageSize = rowSize * infoHeader.height;
    
    file.write(reinterpret_cast<const char*>(&writeFileHeader), sizeof(BitmapFileHeader));
    file.write(reinterpret_cast<const char*>(&writeInfoHeader), sizeof(BitmapInfoHeader));
    
    return true;
}

bool BMPProcessor::writeImageData(ofstream& file) const {
    int bytesPerRow = infoHeader.width * (is32Bit ? 4 : 3);
    int padding = (4 - (bytesPerRow % 4)) % 4;
    
    for (int y = infoHeader.height - 1; y >= 0; --y) {
        for (int x = 0; x < infoHeader.width; ++x) {
            const Pixel& pixel = imageData[y][x];
            
            if (is32Bit) {
                file.write(reinterpret_cast<const char*>(&pixel), 4);
            } else {
                file.write(reinterpret_cast<const char*>(&pixel), 3);
            }
        }
        
        if (!is32Bit && padding > 0) {
            char paddingBytes[4] = {0};
            file.write(paddingBytes, padding);
        }
    }
    
    return true;
}

void BMPProcessor::displayImage() const {
    if (!isLoaded()) {
        cout << "Нет загруженного изображения" << endl;
        return;
    }
    
    cout << "\nИзображение " << infoHeader.width << "x" << infoHeader.height << ":\n";
    cout << string(infoHeader.width * 2, '-') << endl;
    
    for (int y = 0; y < infoHeader.height; ++y) {
        for (int x = 0; x < infoHeader.width; ++x) {
            cout << pixelToChar(imageData[y][x]) << " ";
        }
        cout << endl;
    }
    
    cout << string(infoHeader.width * 2, '-') << endl;
}

char BMPProcessor::pixelToChar(const Pixel& pixel) const {
    if (pixel.red == 0 && pixel.green == 0 && pixel.blue == 0) {
        return '#';
    } else if (pixel.red == 255 && pixel.green == 255 && pixel.blue == 255) {
        return '.';
    } else {
        return '?';
    }
}

bool BMPProcessor::isValidPixel(const Pixel& pixel) const {
    return (pixel.red == 0 && pixel.green == 0 && pixel.blue == 0) ||
           (pixel.red == 255 && pixel.green == 255 && pixel.blue == 255);
}

void BMPProcessor::drawCross() {
    if (!isLoaded()) {
        cout << "Нет загруженного изображения" << endl;
        return;
    }
    
    Pixel blackColor = {0, 0, 0, 255};
    
    int width = infoHeader.width;
    int height = infoHeader.height;
    
    drawLine(0, 0, width - 1, height - 1, blackColor);
    drawLine(width - 1, 0, 0, height - 1, blackColor);
}

void BMPProcessor::drawLine(int x1, int y1, int x2, int y2, const Pixel& color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    int x = x1, y = y1;
    
    while (true) {
        if (x >= 0 && x < infoHeader.width && y >= 0 && y < infoHeader.height) {
            imageData[y][x] = color;
        }
        
        if (x == x2 && y == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
} 