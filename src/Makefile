CC = clang++
BLOODWORD = BloodSword.cpp
BLOODWORD_OUTPUT = BloodSword.exe
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -O3 -std=c++17 -lstdc++fs
INCLUDES = -I/usr/include/SDL2

all: clean blood-sword

blood-sword:
	$(CC) -Wall $(BLOODWORD) $(LINKER_FLAGS) $(INCLUDES) -o $(BLOODWORD_OUTPUT)

debug:
	$(CC) -Wall $(BLOODWORD) $(LINKER_FLAGS) $(INCLUDES) -o $(BLOODWORD_OUTPUT) -DDEBUG

clean:
	rm -f *.exe *.o
