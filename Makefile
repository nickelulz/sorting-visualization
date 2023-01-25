CC := g++
CFLAGS = -w -g -Wall -std=c++17
CFLAGS += -ldl -lpthread -Ilib/loguru -lsfml-graphics -lsfml-window -lsfml-system

SRC  = $(wildcard src/**/*.cpp) $(wildcard src/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp)
OBJ  = $(SRC:.cpp=.o)
BIN = bin

.PHONY: build clean

build: dirs exe

dirs:
	mkdir -p ./$(BIN)

run: exe
	$(BIN)/exe

exe: $(OBJ)
	$(CC) -o $(BIN)/exe $^ $(CFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

paper:
	cd design_log && make

clean:
	rm -rf $(BIN)/* $(OBJ) && cd design_log && make clean