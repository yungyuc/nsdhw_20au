.PHONY: clean test

CXX=g++

_vector.cpython-38-x86_64-linux-gnu.so: _vector.cpp
	$(CXX) -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _vector.cpp -o _vector`python3-config --extension-suffix`

clean:
	rm -rf *.o  *.so .pytest_cache __pycache__/

test: _vector.cpython-38-x86_64-linux-gnu.so
	pytest

