#!/usr/bin/env python3
"""
Скрипт для создания более крупного демонстрационного BMP файла
"""

import struct

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

def create_larger_demo():
    """Создает 20x20 изображение с более четким паттерном"""
    width, height = 20, 20
    header = create_bmp_header(width, height)
    
    data = b''
    for y in range(height - 1, -1, -1):
        row_data = b''
        for x in range(width):
            # Создаем более интересный паттерн
            if (5 <= x <= 14 and 5 <= y <= 14):
                # Центральный квадрат
                row_data += create_black_pixel()
            elif (x == 0 or x == width-1 or y == 0 or y == height-1):
                # Рамка
                row_data += create_black_pixel()
            else:
                row_data += create_white_pixel()
        
        padding_size = (4 - (len(row_data) % 4)) % 4
        row_data += b'\x00' * padding_size
        data += row_data
    
    return header + data

def main():
    print("Создание крупного демонстрационного BMP файла...")
    
    with open("large_demo.bmp", 'wb') as f:
        f.write(create_larger_demo())
    print("Создан: large_demo.bmp - 20x20 изображение с рамкой и квадратом")
    
    print("\nТеперь можно протестировать:")
    print("make && echo -e 'large_demo.bmp\\nlarge_result.bmp' | ./bmp_processor")

if __name__ == "__main__":
    main() 