# running ``make`` should build your code.  ``make test`` runs the Python-based
# tests.  ``make clean`` removes all the built and intermediate files.
#
CXX = c++

calcAngle:
	$(CXX) -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` calcAngle.cpp -o calcAngle`python3-config --extension-suffix`


.PHONY:
clean:
	rm -rf calcAngle *.o *so

test:
	pytest

