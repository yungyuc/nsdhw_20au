CXX = g++
CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC

MODULE = _vector
SOFILE = $(MODULE)$(shell python3-config --extension-suffix)

.PHONY: all test clean
all: $(SOFILE)

$(SOFILE): angle.cc
	$(CXX) $(CXXFLAGS) `python3 -m pybind11 --includes` angle.cc -o $@

test: $(SOFILE)
	@pytest

clean:
	@rm -rf $(SOFILE) __pycache__ .pytest_cache
