# Ref: https://pybind11.readthedocs.io/en/latest/compiling.html#building-manually
CXX = clang++
CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC

PYTHON = python3

SRC = _vector

all: $(SRC).*.so

$(SRC).*.so: $(SRC).cpp
	$(CXX) $(CXXFLAGS) `$(PYTHON) -m pybind11 --includes` $< -o $(SRC)`$(PYTHON)-config --extension-suffix`

.PHONY: test
test: all
	$(PYTHON) -m pytest -v

.PHONY: clean
clean:
	$(RM) _vector *.so
