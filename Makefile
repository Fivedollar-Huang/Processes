
out: main.o structures.o functions.o handleFile.o
	g++ main.o structures.o functions.o handleFile.o -o out
	rm *.o

main.o: main.cpp
	g++ -c main.cpp

structures.o: structures.cpp structures.hpp
	g++ -c structures.cpp

functions.o: functions.cpp functions.hpp
	g++ -c functions.cpp

handleFile.o: handleFile.cpp handleFile.hpp
	g++ -c handleFile.cpp

clean:
	rm *.o out

