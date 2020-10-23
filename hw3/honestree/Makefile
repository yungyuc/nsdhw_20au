.PHONY: clean test _matrix

FLAGS ?=
MKLROOT ?= ${HOME}/opt/conda
MKLEXT ?= so
MKLLINKLINE := \
	${MKLROOT}/lib/intel64/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/intel64/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/intel64/libmkl_core.${MKLEXT} \
	-lpthread -lm -ldl

CXX = g++
CXXFLAGS := -Wl,--no-as-needed \
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

