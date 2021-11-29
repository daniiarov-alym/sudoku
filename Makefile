out: main.o gameWindow.o board.o
	mkdir -p bin
	g++ -o bin/sudoku main.o gameWindow.o board.o -lncurses

main.o: ./src/main.cpp
	g++ -c -g ./src/main.cpp
	
gameWindow.o: ./src/gameWindow.hpp ./src/gameWindow.cpp
	g++	-c -g ./src/gameWindow.cpp
	
board.o: ./src/board.hpp ./src/board.cpp
	g++ -c -g ./src/board.cpp
	
		
clean:
	rm *.o