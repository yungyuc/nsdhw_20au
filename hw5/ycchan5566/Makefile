C = g++
MODULE = _matrix
SOFILE = $(MODULE)$(shell python3-config --extension-suffix)
SRC = matrix.o ../mod.o

INCLUDE_DIRS = ${HOME}/opt/conda/include
MKL_DIR ?= ${HOME}/opt/conda/lib

ifneq (,$(wildcard /opt/intel/mkl/.))
	INCLUDE_DIRS = /opt/intel/mkl/include
	MKL_DIR = /opt/intel/mkl/lib/intel64
endif

MKLFILE := \
    ${MKL_DIR}/libmkl_def.so \
    ${MKL_DIR}/libmkl_avx2.so \
    ${MKL_DIR}/libmkl_intel_lp64.so \
    ${MKL_DIR}/libmkl_sequential.so \
    ${MKL_DIR}/libmkl_core.so \
	-lpthread -lm -ldl

NUMPYINC := $(shell python3 -c "import numpy; print(numpy.get_include())")

CXXFLAGS := -O3 -Wl,--no-as-needed \
	-std=c++17 -O3 -g -m64 \
	-Wall -Wextra -shared \
	-fPIC `python3 -m pybind11 --includes` \
	-I$(INCLUDE_DIRS) \
	-I$(NUMPYINC) \
	-I./

.PHONY: clean
$(SOFILE): $(SRC)
	${C} ${CXXFLAGS} $(MKLFILE) $? -o $@

clean:
	@rm -Rfv $(SOFILE) __pycache__ .pytest_cache $(SRC)
