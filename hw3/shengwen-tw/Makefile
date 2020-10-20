EXECUTABLE=hw3
CC=g++

MKL_PATH=/opt/intel/mkl
MKL_LDFLAGS=$(MKL_PATH)/lib/intel64/libmkl_intel_lp64.so \
	$(MKL_PATH)/lib/intel64/libmkl_sequential.so \
	$(MKL_PATH)/lib/intel64/libmkl_core.so

CFLAGS=-O3 -Wall -g

LDFLAGS=-lm

CFLAGS+=-I$(MKL_PATH)/include

SRC=main.cpp

OBJS=$(SRC:.cpp=.o)
DEPEND=$(SRC:.cpp=.d)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	@echo "LD" $@
	@$(CC) $(CFLAGS) $(OBJS) $(MKL_LDFLAGS) $(LDFLAGS) -o $@

-include $(DEPEND)

%.o: %.cpp
	@echo "CC" $@
	@$(CC) $(CFLAGS) -MMD -MP -c $< $(LDFLAGS) -o $@

clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(OBJS)

.PHONY: all clean
