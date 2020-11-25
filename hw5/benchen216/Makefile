FLAGS ?=

UNAME_S := $(shell uname -s)
NUMPY_PATH := $(shell python3 -c "import numpy;print(numpy.get_include())")
ifeq ($(UNAME_S),Darwin)
MKLROOT ?= /opt/intel/mkl
MKLEXT ?= a
CXXFLAGS :=
endif

ifeq ($(UNAME_S),Linux)
MKLROOT ?= ${HOME}/opt/conda
#MKLROOT ?= /home/ben/anaconda3/envs/nsd
MKLEXT ?= so
CXXFLAGS := -Wl,--no-as-needed
endif

MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	-lpthread -lm -ldl -fPIC
PYBINDLIB := `python3 -m pybind11 --includes`
CXX = g++
CXXFLAGS := -O3 -Wall -shared -std=c++17 \
	-fPIC `python3 -m pybind11 --includes` ../mod.cpp \
	-o _matrix`python3-config --extension-suffix` \
	-Wl,--no-as-needed \
	-I ./ \
	-I${NUMPY_PATH} \
	-I${MKLROOT}/include \
	${MKLLINKLINE}


.PHONY: default
default: matrix.hpp
	${CXX} ${CXXFLAGS}

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ ${BINS} *.so *.txt __pycache__

.PHONY: test
test:
	make;pytest -v
