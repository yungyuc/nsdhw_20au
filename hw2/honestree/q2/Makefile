.PHONY: clean test

CXX=g++

libshared_vector.so: _vector.hpp
	$(CXX) -c _vector.hpp --std=c++11 -Wall -Wextra -o _vector.o
	$(CXX) -shared _vector.o --std=c++11 -Wall -Wextra -o libshared_vector.so


clean:
	rm -rf *.o  *.so .pytest_cache __pycache__/

test: libshared_vector.so
	pytest

