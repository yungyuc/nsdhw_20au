CXX = g++

.PHONY: default
default: q1

q1: q1.o
	$(CXX) $^ -o $@

q1.o: q1.cpp
	$(CXX) -c $< -o $@

.PHONY: run
run:
	./q1 > result.txt

.PHONY: clean
clean:
	rm -rf q1 result.txt *.o
