#!/usr/bin/env python3
"""
Скрипт для создания демонстрационных BMP файлов
Создает пару изображений: исходное и с нарисованным крестом
"""

import struct
import os

def create_bmp_header(width, height, bits_per_pixel=24):
    bytes_per_pixel = bits_per_pixel // 8
    row_size = width * bytes_per_pixel
    padding = (4 - (row_size % 4)) % 4
    row_size_with_padding = row_size + padding
    image_size = row_size_with_padding * height
    
    file_header = struct.pack('<HIHHI',
        0x4D42,
        14 + 40 + image_size,
        0,
        0,
        14 + 40
    )
    
    info_header = struct.pack('<IiiHHIIiiII',
        40,
        width,
        height,
        1,
        bits_per_pixel,
        0,
        image_size,
        0,
        0,
        0,
        0
    )
    
    return file_header + info_header

def create_white_pixel():
    return struct.pack('<BBB', 255, 255, 255)

def create_black_pixel():
    return struct.pack('<BBB', 0, 0, 0)

def create_demo_image():
    """Создает 10x10 изображение с черным квадратом в центре"""
    width, height = 10, 10
    header = create_bmp_header(width, height)
    
    data = b''
    for y in range(height - 1, -1, -1):
        row_data = b''
        for x in range(width):
            if 3 <= x <= 6 and 3 <= y <= 6:
                row_data += create_black_pixel()
            else:
                row_data += create_white_pixel()
        
        padding_size = (4 - (len(row_data) % 4)) % 4
        row_data += b'\x00' * padding_size
        data += row_data
    
    return header + data

def create_demo_image_with_cross():
    """Создает то же изображение, но с нарисованным крестом"""
    width, height = 10, 10
    header = create_bmp_header(width, height)
    
    data = b''
    for y in range(height - 1, -1, -1):
        row_data = b''
        for x in range(width):
            # Черный квадрат в центре
            if 3 <= x <= 6 and 3 <= y <= 6:
                row_data += create_black_pixel()
            # Диагональный крест
            elif x == y or x == (width - 1 - y):
                row_data += create_black_pixel()
            else:
                row_data += create_white_pixel()
        
        padding_size = (4 - (len(row_data) % 4)) % 4
        row_data += b'\x00' * padding_size
        data += row_data
    
    return header + data

def main():
    print("Создание демонстрационных BMP файлов...")
    
    # Создаем исходное изображение
    with open("demo_before.bmp", 'wb') as f:
        f.write(create_demo_image())
    print("Создан: demo_before.bmp - исходное изображение")
    
    # Создаем изображение с крестом
    with open("demo_after.bmp", 'wb') as f:
        f.write(create_demo_image_with_cross())
    print("Создан: demo_after.bmp - изображение с крестом")
    
    print("\nДемонстрационные файлы готовы для тестирования!")

if __name__ == "__main__":
    main() 