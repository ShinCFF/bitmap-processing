CXX       := g++
CXX_FLAGS := -std=c++17 -ggdb

BIN       := bin
DIST	  := dist
TARGET    := bmp

SRCS      := $(wildcard src/*.cpp)

all: $(BIN)/$(TARGET)

$(BIN)/$(TARGET): $(SRCS)
	$(CXX) $(CXX_FLAGS) $^ -o $@

clean:
	rm -rf $(BIN)/* $(DIST)/*

run: clean
	mkdir -p $(BIN) $(DIST)
	make