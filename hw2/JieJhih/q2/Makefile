MODULE = _vector
SOFILE = $(MODULE)$(shell python3-config --extension-suffix)

.PHONY: all test clean
all: $(SOFILE)

$(SOFILE): angle.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` angle.cpp -o $@

test: $(SOFILE)
	@pytest

clean:
	@rm -rf $(MODULE)`python3-config --extension-suffix` __pycache__ .pytest_cache _vector*