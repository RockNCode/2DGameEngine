build:
	g++ -Wall -std=c++17 -lsdl2 -lsdl2_image -lsdl2_ttf -lsdl2_mixer -llua -I"./libs" src/*.cpp -o  gameengine;

run:
	./gameengine

clean:
	rm gameengine