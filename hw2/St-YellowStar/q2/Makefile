CXX = g++
FLAGS = -O3 -Wall -shared -std=c++11 -fPIC
PYBINDINCLUDE = `python3 -m pybind11 --includes`
PYBINDSUFFIX = `python3-config --extension-suffix`

OBJECT = vector.cpp

bulid:
	$(CXX) $(FLAGS) $(PYBINDINCLUDE) $(OBJECT) -o _vector$(PYBINDSUFFIX) 

clean:
	rm -f *.so

test:
	echo "test"
