CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -Iinclude
LDLIBS  = -lm

SRC_DIR = src
INC_DIR = include
OBJ_DIR = build
BIN     = HW06

SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/config_parser.c \
       $(SRC_DIR)/daxpy_chunks.c \
       $(SRC_DIR)/hdf5_writer.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

HDF5_CFLAGS := $(shell pkg-config --cflags hdf5-serial 2>/dev/null || pkg-config --cflags hdf5 2>/dev/null)
HDF5_LIBS   := $(shell pkg-config --libs   hdf5-serial 2>/dev/null || pkg-config --libs   hdf5 2>/dev/null)

ifeq ($(strip $(HDF5_LIBS)),)
    HDF5_LIBS := -lhdf5
endif

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(HDF5_CFLAGS) -o $@ $^ $(LDLIBS) $(HDF5_LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(HDF5_CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

run: $(BIN)
	./$(BIN) config.txt

clean:
	rm -rf $(OBJ_DIR) $(BIN) output.h5

.PHONY: all run clean
