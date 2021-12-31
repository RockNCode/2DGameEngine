# Variables

CC= g++
COMPILER_FLAGS= -Wall -Wfatal-errors
LANG_STD= -std=c++17
LFLAGS= -lsdl2 -lsdl2_image -lsdl2_ttf -lsdl2_mixer -llua
INCLUDEDDIR=-I"./libs"
SRCDIR =src/*.cpp \
		src/Game/*.cpp \
		src/Logger/*.cpp
OUTPUT=gameengine

build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(LFLAGS) $(INCLUDEDDIR) $(SRCDIR) -o $(OUTPUT);

run:
	./$(OUTPUT)

clean:
	rm $(OUTPUT)