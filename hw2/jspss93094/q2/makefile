all:vangle.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` vangle.cpp -o _vector`python3-config --extension-suffix`
test:
	pytest
clean:
	rm -f *.so; rm -rf __pycache__
