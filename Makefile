CXX = clang++
# export PKG_CONFIG ?= pkg-config

CPPFLAGS = -Wall -Wextra --std=c++17 --pedantic
DEBUG_FLAGS = -fsanitize=address,undefined -g3

SRC = $(shell find src/ -type f -name '*.cc')
OBJ = $(SRC:.cc=.o)

TARGET = reki

build: $(OBJ)
	$(CXX) $(CFLAGS) $(OBJ) $(LD_FLAGS) -o $(TARGET)

debug: $(OBJ)
	$(CXX) $(CFLAGS) $(DEBUG_FLAGS) $(OBJ) $(LD_FLAGS) -o $(TARGET)

clean:
	rm $(OBJ) $(TARGET)

.PHONY: build debug clean
