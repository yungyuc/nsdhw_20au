CC = c++
CFLAGS = -O3 -Wall -Wextra -shared -std=c++11 -fPIC
PY_INC = `python3 -m pybind11 --includes`
SUFFIX = $(shell python3-config --extension-suffix)
TARGET = _vector
LIBRARY = $(TARGET)$(SUFFIX)

.PHONY: clean test
default: $(LIBRARY)

$(LIBRARY): $(TARGET).cpp
	$(CC) $(CFLAGS) $(PY_INC) $? -o $@

test: $(LIBRARY)
	python3 -m pytest

clean:
	rm -Rf $(LIBRARY) __pycache__ .pytest_cache .cache
