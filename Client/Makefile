CC = g++
CFLAGS = -Wall -g -std=c++14

SRC_DIR = ./Src
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp) 

OBJ_DIR = ./Obj
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES)) 

INCLUDES = ../thirdParty/restbed/distribution/include ../thirdParty/json/include
LIBRARIES = ../thirdParty/restbed/distribution/library
LIBS = -lrestbed

BIN_DIR = ./Bin
TARGET_BINARY = $(BIN_DIR)/clientRestApi.bin

all: $(TARGET_BINARY)
	@echo "[Build log] Target binary successfully built!"

$(TARGET_BINARY): $(OBJ_FILES)
	@echo "[Build log] Linking..."
	$(CC) $^ -L$(LIBRARIES) $(LIBS) -o $@
	@echo "[Build log] Program linked!" 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "[Build log] Compiling..."
	$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $@
	@echo "[Build log] Program compiled!"

clean:
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(BIN_DIR)/*.bin

