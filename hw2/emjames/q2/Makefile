# running ``make`` should build your code.  ``make test`` runs the Python-based
# tests.  ``make clean`` removes all the built and intermediate files.
#
CXX = c++

_vector:
	$(CXX) -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _vector.cpp -o _vector`python3-config --extension-suffix`


.PHONY:
clean:
	rm -rf _vector*.o *so

test:
	pytest

