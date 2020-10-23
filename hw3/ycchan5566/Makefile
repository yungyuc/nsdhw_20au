all: pybind_matrix.cpp
	g++ -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` pybind_matrix.cpp -o _matrix`python3-config --extension-suffix`

test: test_matrix.py
	pytest test_matrix.py

clean:
	rm -rf *.so .pytest_cache __pycache__