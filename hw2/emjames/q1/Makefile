# target: <prerequisite> 
# prereqs are optional
#
CXX = g++

main: main.o Line.o
	$(CXX) main.o Line.o -o main

Line.o: Line.cpp
	$(CXX) -c Line.cpp -o Line.o

main.o: main.cpp
	$(CXX) -c main.cpp -o main.o

.PHONY: clean
clean:
	rm -rf main *.o *.txt
run:
	make && ./main
check:
	make && ./main > result.txt
