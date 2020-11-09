all: trabformigas

trabformigas: main.o Formigas.o
#	g++ main.o Formigas.o -o IARformigas -lsfml-graphics -lsfml-window -lsfml-system
	g++ -L./SFML-2.5.1/lib main.o Formigas.o -o trabformigas -lsfml-graphics -lsfml-window -lsfml-system -Wl,-R./SFML-2.5.1/lib

main.o: main.cpp
#	g++ -I./SFML-2.5.1/include -L./SFML-2.5.1/lib -c main.cpp -o main.o 
	g++ -O3 -I./SFML-2.5.1/include -c main.cpp -o main.o

Formigas.o: Formigas.cpp Formigas.hpp
	g++ -O3 -c Formigas.cpp -o Formigas.o
 
.PHONY: clean mrproper

clean: 
	rm -f Formigas.o  main.o

mrproper: clean
	rm  -f trabformigas
