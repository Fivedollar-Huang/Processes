
out: main.o structures.o functions.o handleFile.o
	g++ main.o structures.o functions.o handleFile.o -o out
	rm *.o

main.o: main.cpp
	g++ -c main.cpp

structures.o: src/structures.cpp src/structures.hpp
	g++ -c src/structures.cpp

functions.o: src/functions.cpp src/functions.hpp
	g++ -c src/functions.cpp

handleFile.o: src/handleFile.cpp src/handleFile.hpp
	g++ -c src/handleFile.cpp

clean:
	rm *.o out

