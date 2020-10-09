EXECUTABLE=../q1/hw2

all:
	mkdir -p ../q1
	g++ -o $(EXECUTABLE) main.cpp

run:
	./$(EXECUTABLE)

check:
	./$(EXECUTABLE) > result.txt

clean:
	rm -rf $(EXECUTABLE)

.PHONY: all run check clean
