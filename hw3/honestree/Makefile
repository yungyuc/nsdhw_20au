.PHONY: clean test _matrix

FLAGS ?=

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
MKLROOT ?= /opt/intel/mkl
MKLEXT ?= a
CXXFLAGS :=
endif

ifeq ($(UNAME_S), Linux)
MKLROOT ?= ${HOME}/opt/conda
MKLEXT ?= so
CXXFLAGS := -Wl,--no-as-needed
endif

MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_def.${MKLEXT} \
	${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	${MKLROOT}/lib/libmkl_avx2.${MKLEXT} \
	-lpthread -lm -ldl

CXX = g++
CXXFLAGS := ${CXXFLAGS}　-Wl,--no-as-needed　\
	-std=c++17 -O3 -g -m64 -Wall \
	-I${MKLROOT}/include \
	${MKLLINKLINE} \
	${FLAGS}

PYBIND :=  	-shared -fPIC `python3 -m pybind11 --includes` _matrix.cpp \
			-o _matrix`python3-config --extension-suffix`

_matrix: _matrix.cpp
	$(CXX) ${CXXFLAGS} ${PYBIND}

clean:
	rm -rf *.o *.dSYM/ ${BINS} *.so .pytest_cache __pycache__/

test: _matrix
	pytest

