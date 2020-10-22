CXX = g++

.PHONY: default
default: q1

q1: q1.o
	$(CXX) $^ -o $@

q1.o: q1.cpp
	$(CXX) -c $< -o $@

.PHONY: check
check:
	./q1 > result.txt

.PHONY: run
run:
	./q1

.PHONY: clean
clean:
	rm -rf q1 result.txt *.o
