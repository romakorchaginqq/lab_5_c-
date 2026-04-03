CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude
TARGET = product_manager
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
DATA_DIR = data

$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR) $(DATA_DIR))

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

.PHONY: all clean run debug help

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJECTS)
 $(CXX) $(CXXFLAGS) $^ -o $@
 @echo "✓ Сборка завершена. Исполняемый файл: $(BIN_DIR)/$(TARGET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
 $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
 rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/$(TARGET)
 @echo "✓ Очистка завершена"

run: $(BIN_DIR)/$(TARGET)
 ./$(BIN_DIR)/$(TARGET)

debug: CXXFLAGS += -g -DDEBUG
debug: clean $(BIN_DIR)/$(TARGET)
 gdb ./$(BIN_DIR)/$(TARGET)

help:
 @echo "Доступные команды:"
 @echo "  make        - Собрать проект"
 @echo "  make clean  - Очистить собранные файлы"
 @echo "  make run    - Собрать и запустить программу"
 @echo "  make debug  - Собрать с отладочной информацией и запустить GDB"
 @echo "  make help   - Показать эту справку"

check:
 @command -v $(CXX) >/dev/null 2>&1 || { echo "❌ $(CXX) не установлен"; exit 1; }
 @echo "✓ Все необходимые инструменты установлены"
