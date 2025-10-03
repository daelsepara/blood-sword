CC=clang++
BLOODWORD=BloodSword.cpp
BLOODWORD_OUTPUT=BloodSword.exe
LIBS=-O3 -std=c++17 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
INCLUDES=-I/usr/include/SDL2
UNAME_S=$(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	LLVM=$(shell brew --prefix llvm)
	CC=$(LLVM)/bin/clang++
	LIBS += -stdlib=libc++ -L$(LLVM)/lib/c++ -L/usr/local/lib -Wl,-dead_strip -fdata-sections -ffunction-sections
	INCLUDES += -I/usr/local/include/SDL2
else
	CC=g++
	LIBS += -lstdc++fs
endif

all: clean blood-sword

blood-sword:
	$(CC) -Wall $(BLOODWORD) $(LIBS) $(INCLUDES) -o $(BLOODWORD_OUTPUT)

debug:
	$(CC) -Wall $(BLOODWORD) $(LIBS) $(INCLUDES) -o $(BLOODWORD_OUTPUT) -DDEBUG

clean:
	rm -f *.exe *.o
