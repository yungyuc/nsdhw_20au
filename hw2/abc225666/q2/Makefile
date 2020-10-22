TARGET = _vector$(shell python3-config --extension-suffix)

$(TARGET): _vector.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _vector.cpp -o _vector`python3-config --extension-suffix`

test: $(TARGET)
	pytest

clean:
	rm -rf $(TARGET) __pycache__ .pytest_cache
