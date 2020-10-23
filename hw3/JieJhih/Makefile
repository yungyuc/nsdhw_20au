C = g++
MODULE = _matrix
SOFILE = $(MODULE)$(shell python3-config --extension-suffix)
SRC= matrix.cpp \
	 matrix.h

INCLUDE_DIRS = ${HOME}/opt/conda/include
MKL_DIR = ${HOME}/opt/conda/lib

ifneq (,$(wildcard /opt/intel/mkl/.))
	INCLUDE_DIRS = /opt/intel/mkl/include
	MKL_DIR = /opt/intel/mkl/lib/intel64
endif

CFLAGS = -O3 -Wl,--no-as-needed -shared -std=c++11 -fPIC -I$(INCLUDE_DIRS) -ldl -lpthread -lm \
			$(MKL_DIR)/libmkl_def.so \
			$(MKL_DIR)/libmkl_avx2.so \
			$(MKL_DIR)/libmkl_core.so \
			$(MKL_DIR)/libmkl_intel_lp64.so \
			$(MKL_DIR)/libmkl_sequential.so

.PHONY: all test clean
all: $(SOFILE)

$(SOFILE): $(SRC)
	$(C) `python3 -m pybind11 --includes` -o $@ $(CFLAGS)

test: $(SOFILE)
	python3 -m pytest -v -l

clean:
	@rm -rf $(SOFILE) __pycache__ .pytest_cache performance.txt
