CXX = clang++

PYTHON = python3

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
    ${MKLROOT}/lib/libmkl_def.${MKLEXT} \
    ${MKLROOT}/lib/libmkl_avx2.${MKLEXT} \
    ${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
    ${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
    ${MKLROOT}/lib/libmkl_core.${MKLEXT} \
    -lpthread -lm -ldl

CXXFLAGS := ${CXXFLAGS} \
    -std=c++17 -O3 -g -m64 \
	-ftree-vectorize -funroll-loops -ffast-math \
    -I${MKLROOT}/include \
    ${MKLLINKLINE} -shared -fPIC

BIN := _matrix

.PHONY: default
default: ${BIN}.*.so

${BIN}.*.so: $(BIN).cpp
	$(CXX) $(CXXFLAGS) `$(PYTHON) -m pybind11 --includes` $< -o $(BIN)`$(PYTHON)-config --extension-suffix`

.PHONY: clean
clean:
	$(RM) ${BIN} *.so

.PHONY: test
test: default
	python3 -m pytest -v
