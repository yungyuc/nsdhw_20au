CXX = g++
CXXFLAGS = -std=c++17 -Wall

OBJECTS = main.o line.o

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJECTS): line.hpp

clean:
	rm -f main ${OBJECTS} result.txt
run:
	if [ -f "main" ]; then \
		./main; \
	else \
		make main; \
		./main; \
	fi;
check:
	if [ -f "main" ]; then \
		./main > result.txt; \
	else \
		make main; \
		./main > result.txt; \
	fi;
