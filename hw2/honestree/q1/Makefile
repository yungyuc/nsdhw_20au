.PHONY: clean run check

CXX=g++

line: Line.hpp main.cpp
	$(CXX) main.cpp --std=c++11 -Wall -Wextra -o line

clean:
	rm -f line
	rm -f result.txt

run: line
	./line

check: line
	./line > result.txt
