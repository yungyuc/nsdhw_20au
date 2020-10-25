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
	${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	${MKLROOT}/lib/libmkl_avx2.${MKLEXT} \
    ${MKLROOT}/lib/libmkl_def.${MKLEXT} \
	-lpthread -lm -ldl

CXX = g++
CXXFLAGS := -O3 -Wall -shared -std=c++11 \
	-fPIC `python3 -m pybind11 --includes` matrix.cpp \
	-o _matrix`python3-config --extension-suffix` \
	-Wl,--no-as-needed \
	-I${MKLROOT}/include \
	${MKLLINKLINE}
	
.PHONY: default test clean
default:matrix.cpp
	${CXX} ${CXXFLAGS}
test: default
	pytest
clean:
	rm -f *.so; rm -rf __pycache__; rm -rf performance.txt 
