FLAGS ?=

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
MKLROOT ?= /opt/intel/mkl
MKLEXT ?= a
CXXFLAGS :=
endif

ifeq ($(UNAME_S),Linux)
MKLROOT ?= ${HOME}/opt/conda
MKLEXT ?= so
CXXFLAGS := -Wl,--no-as-needed
endif

MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	-lpthread -lm -ldl

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -O3 -g -m64 \
	-Wall -Wextra -Werror -shared \
	-fPIC `python3 -m pybind11 --includes` \
	-I${MKLROOT}/include \
	${MKLLINKLINE} \
	${FLAGS}

TARGET = _matrix
LIBRARY = $(TARGET)$(shell python3-config --extension-suffix)

.PHONY: clean test
$(LIBRARY): _matrix.cpp
	echo $@
	${CXX} ${CXXFLAGS} $? -o $@

clean:
	rm -Rf $(LIBRARY) __pycache__ .pytest_cache

test: $(LIBRARY)
	../validate.py
