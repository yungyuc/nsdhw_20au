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

NUMPYINC := $(shell python3 -c "import numpy; print(numpy.get_include())")

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -O3 -g -m64 \
	-Wall -Wextra -Werror -shared \
	-fPIC `python3 -m pybind11 --includes` \
	-I${MKLROOT}/include \
	-I$(NUMPYINC) \
	-I./ \
	${FLAGS}
LDFLAGS = ${MKLLINKLINE}

TARGET = _matrix
OBJS = matrix.o
LIBRARY = $(TARGET)$(shell python3-config --extension-suffix)

.PHONY: clean test
$(LIBRARY): $(OBJS)
	${CXX} ${CXXFLAGS} $(LDFLAGS) $? -o $@

clean:
	@rm -Rfv $(LIBRARY) __pycache__ .pytest_cache
	@rm -Rfv $(OBJS)

test: $(LIBRARY)
	../validate.py
