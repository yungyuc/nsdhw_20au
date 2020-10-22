CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -O3

.PHONY: all run check clean
all: main

main: main.cc line.cc line.h
	$(CXX) $(CXXFLAGS) -o $@ main.cc line.cc

run: main
	./main

check: main
	./main > result.txt

clean:
	@rm main result.txt
