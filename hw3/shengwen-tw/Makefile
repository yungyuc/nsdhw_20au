EXECUTABLE=hw3
SHARED_OBJ=_matrix.so
CC=g++

#on my pc
MKL_PATH=/opt/intel/mkl
MKL_LDFLAGS=$(MKL_PATH)/lib/intel64/libmkl_intel_lp64.so \
	$(MKL_PATH)/lib/intel64/libmkl_sequential.so \
	$(MKL_PATH)/lib/intel64/libmkl_core.so

#on ec2
#MKL_PATH=$(HOME)/opt/conda
#MKL_LDFLAGS=-L$(MKL_PATH)/lib -lmkl_rt

CFLAGS=-O3 -Wall -g

LDFLAGS=-lm

CFLAGS+=-I$(MKL_PATH)/include

PYBIND_CFLAGS=-shared -std=c++11 -fPIC \
	`python3 -m pybind11 --includes`
PYBIND_LDFLAGS=`python3-config --ldflags`

SRC=main.cpp \
	matrix.cpp

PYBIND_SRC=share.cpp

OBJS=$(SRC:.cpp=.o)
DEPEND=$(SRC:.cpp=.d)

all: $(EXECUTABLE) $(SHARED_OBJ)

$(EXECUTABLE): $(OBJS)
	@echo "LD" $@
	@$(CC) $(CFLAGS) $(OBJS) $(MKL_LDFLAGS) $(LDFLAGS) -o $@

$(SHARED_OBJ): $(SRC) $(PYBIND_SRC)
	@echo "LD" $@
	@$(CC) $(CFLAGS) $(PYBIND_CFLAGS) $(SRC) $(PYBIND_SRC) $(MKL_LDFLAGS) $(LDFLAGS) $(PYBIND_LDFLAGS) -o $@

-include $(DEPEND)

%.o: %.cpp
	@echo "CC" $@
	@$(CC) $(CFLAGS) -MMD -MP -c $< $(LDFLAGS) -o $@

test: $(EXECUTABLE) $(SHARED_OBJ)
	@echo 'start generating `performance.txt`'
	@./hw3 > performance.txt
	@echo ''
	@echo 'finish generating `performance.txt`'

clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(SHARED_OBJ)
	rm -rf $(OBJS)

.PHONY: all clean test
