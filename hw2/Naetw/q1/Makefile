CC = clang++
CFLAGS = -Wall -std=gnu++14 -g
INCLUDE = -Iinclude

LINEDIR = lib/
LINE := $(shell find $(LINEDIR) -name '*.cpp')

SRC := $(LINE)

EXEC = line
OUTPUTFILE = result.txt
OBJS = main.cpp \
	   $(SRC)

# Substitution reference
DEPS := $(OBJS:%.cpp=%.d)
OBJS := $(OBJS:%.cpp=%.o)

all: $(EXEC)

# Static pattern rule
%.o: %.cpp
	$(CC) -o $@ $(CFLAGS) $(INCLUDE) -c -MMD $<

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(INCLUDE)

.PHONY: run
run: all
	./$(EXEC)

.PHONY: check
check: all
	./$(EXEC) > $(OUTPUTFILE)

.PHONY: clean
clean:
	$(RM) $(DEPS) $(OBJS) $(EXEC) $(OUTPUTFILE)

-include $(DEPS)
