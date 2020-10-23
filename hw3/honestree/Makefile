.PHONY: clean test _matrix

INCLUDE_DIRS = ${HOME}/opt/conda/include	
MKLROOT = ${HOME}/opt/conda/lib


MKLLINKLINE := \
	${MKLROOT}/libmkl_def.so \
	${MKLROOT}/libmkl_intel_lp64.so \
	${MKLROOT}/libmkl_sequential.so \
	${MKLROOT}/libmkl_core.so \
	${MKLROOT}/libmkl_avx2.so \
	-lpthread -lm -ldl

CXX = g++
CXXFLAGS := -Wl,--no-as-needed \
	-std=c++17 -O3 -g -m64 -Wall \
	-I${MKLROOT}/include \
	${MKLLINKLINE} \
	${FLAGS}

PYBIND :=  	-shared -fPIC `python3 -m pybind11 --includes` _matrix.cpp \
			-o _matrix.cpython-37m-x86_64-linux-gnu.so

_matrix: _matrix.cpp
	g++ -Wl,--no-as-needed -std=c++17 -O3 -g -m64 -Wall -Wextra -Werror -shared -lpthread -lm -ldl \
	-I/home/runner/opt/conda/include \
	/home/runner/opt/conda/lib/libmkl_intel_lp64.so \
	/home/runner/opt/conda/lib/libmkl_sequential.so \
	/home/runner/opt/conda/lib/libmkl_core.so \
	/home/runner/opt/conda/lib/libmkl_avx512 \
	/home/runner/opt/conda/lib/libmkl_def.so \
	${PYBIND}

clean:
	rm -rf *.o *.dSYM/ ${BINS} *.so .pytest_cache __pycache__/

test: _matrix
	pytest

