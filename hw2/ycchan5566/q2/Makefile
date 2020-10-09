all: pybind_vector.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` pybind_vector.cpp -o _vector`python3-config --extension-suffix`

test: test_vec.py
	pytest test_vec.py

clean:
	rm -rf *.so .pytest_cache __pycache__