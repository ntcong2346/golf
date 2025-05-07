# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -I./include
LDFLAGS = -L./lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Files
SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/init.cpp $(SRC_DIR)/render.cpp $(SRC_DIR)/update.cpp
OBJS = $(SRC_DIR)/main.o $(SRC_DIR)/init.o $(SRC_DIR)/render.o $(SRC_DIR)/update.o
TARGET = main.exe

# Rules
all: directories $(TARGET)

directories:
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	@if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)
	@if exist $(SRC_DIR)/*.o del $(SRC_DIR)/*.o
	@if exist $(TARGET) del $(TARGET)

run: all
	$(TARGET)

.PHONY: all clean run directories