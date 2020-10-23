.PHONY: clean test _matrix

ifneq ($(wildcard /opt/intel/mkl/.),)
	INCLUDE_DIRS = /opt/intel/mkl/include
	MKLROOT = /opt/intel/mkl/lib/intel64
else
	INCLUDE_DIRS = ${HOME}/opt/conda/include
	MKLROOT = ${HOME}/opt/conda/lib
endif

MKLLINKLINE := \
	${MKLROOT}/libmkl_def.so \
	${MKLROOT}/libmkl_intel_lp64.so \
	${MKLROOT}/libmkl_sequential.so} \
	${MKLROOT}/libmkl_core.so \
	${MKLROOT}/libmkl_avx2.so \
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

