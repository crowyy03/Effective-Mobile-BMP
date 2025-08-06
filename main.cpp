#include "bmp_processor.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    cout << "=== Консольное приложение для работы с BMP файлами ===" << endl;
    cout << "Поддерживаемые форматы: 24 и 32 битные BMP файлы" << endl;
    cout << "Требования: изображения должны содержать только черный и белый цвета" << endl;
    cout << endl;
    
    BMPProcessor processor;
    string inputFilename, outputFilename;
    
    cout << ">> Enter input BMP file name: ";
    getline(cin, inputFilename);
    
    if (!processor.loadImage(inputFilename)) {
        cerr << "Ошибка загрузки файла. Программа завершается." << endl;
        return 1;
    }
    
    cout << "Файл успешно загружен!" << endl;
    cout << "Размер изображения: " << processor.getWidth() << "x" << processor.getHeight() << endl;
    
    cout << "\nИсходное изображение:" << endl;
    processor.displayImage();
    
    cout << "\nРисуем крест на изображении..." << endl;
    processor.drawCross();
    
    cout << "\nМодифицированное изображение:" << endl;
    processor.displayImage();
    
    cout << "\n>> Enter output BMP file name: ";
    getline(cin, outputFilename);
    
    if (!processor.saveImage(outputFilename)) {
        cerr << "Ошибка сохранения файла." << endl;
        return 1;
    }
    
    cout << "Изображение успешно сохранено в файл: " << outputFilename << endl;
    cout << "\nПрограмма завершена." << endl;
    
    return 0;
} 