# Makefile para SDL2 (Windows/MinGW, SDL incluída no projeto)

CC       := gcc
CFLAGS   := -Wall -g \
            -Iinclude \
            -Iinclude/SDL2 \
            -Isrc \
            -Isrc/core 

# Onde estão suas .a e .dll
LIB_DIR      := src/lib
SDL2MAIN_A   := $(LIB_DIR)/libSDL2main.a
SDL2_IMPORT  := $(LIB_DIR)/libSDL2.dll.a
SDL2_DLL     := $(LIB_DIR)/SDL2.dll

SRC_DIR  := src
BUILD    := build
BIN_DIR  := bin

SRCS := \
    $(SRC_DIR)/main.c \
    $(SRC_DIR)/core/algebra.c \
    $(SRC_DIR)/core/objeto.c \
    $(SRC_DIR)/core/camera.c \
    $(SRC_DIR)/core/tela.c

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD)/%.o)

all: $(BIN_DIR)/programa.exe

# 1) Compila cada .c → .o e gera .d
$(BUILD)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

# 2) Linka TODOS objetos, depois as .a, e copia a DLL
$(BIN_DIR)/programa.exe: $(OBJS) $(SDL2MAIN_A) $(SDL2_IMPORT)
	@mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ \
	    -lmingw32 \
	    $(SDL2MAIN_A) \
	    $(SDL2_IMPORT)
	@cp $(SDL2_DLL) $(BIN_DIR)/

# inclui dependências
-include $(OBJS:.o=.d)

clean:
	rm -rf $(BUILD) $(BIN_DIR)

.PHONY: all clean
