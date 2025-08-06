# Makefile для консольного приложения работы с BMP файлами
# Effective Mobile Test Task

# Компилятор
CXX = g++
# Альтернативные компиляторы (раскомментируйте нужный)
# CXX = clang++
# CXX = g++-11

# Флаги компиляции
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g

# Имя исполняемого файла
TARGET = bmp_processor

# Исходные файлы
SOURCES = main.cpp bmp_processor.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Правила сборки
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка
clean:
	rm -f $(OBJECTS) $(TARGET)

# Пересборка
rebuild: clean all

# Установка (копирование в /usr/local/bin)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

# Удаление установленного файла
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)

# Запуск
run: $(TARGET)
	./$(TARGET)

# Проверка синтаксиса
check:
	$(CXX) $(CXXFLAGS) -fsyntax-only $(SOURCES)

# Помощь
help:
	@echo "Доступные команды:"
	@echo "  make        - собрать проект"
	@echo "  make clean  - удалить объектные файлы"
	@echo "  make rebuild- пересобрать проект"
	@echo "  make run    - собрать и запустить"
	@echo "  make check  - проверить синтаксис"
	@echo "  make install- установить в систему"
	@echo "  make uninstall - удалить из системы"

.PHONY: all clean rebuild install uninstall run check help 