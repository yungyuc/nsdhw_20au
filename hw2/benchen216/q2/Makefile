
.PHONY: default
default: run

run:
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _vector.cpp -o _vector`python3-config --extension-suffix`

test:
	pytest q2.py
clean:
	rm -fr *.so .pytest_cache



# vim: set noet nobomb fenc=utf8 ff=unix:
