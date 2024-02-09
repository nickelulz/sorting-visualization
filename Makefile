CC := g++
CFLAGS = -w -g -Wall -std=c++17
CFLAGS += -ldl -lpthread -Ilib/loguru -lsfml-graphics -lsfml-window -lsfml-system

SRC = main.cpp 
OBJ = main.o
EXE = executable

.PHONY: build clean

build: ${EXE}

run: ${EXE}
	./${EXE}

${EXE}: $(OBJ)
	$(CC) -o $(EXE) $^ $(CFLAGS)
	rm $(OBJ)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

paper:
	cd design_log && make

clean:
	rm -rf $(OBJ) $(EXE)
