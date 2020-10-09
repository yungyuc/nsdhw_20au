EXECUTABLE=hw2

SRC=main.cpp

$(EXECUTABLE): $(SRC)
	g++ -o $(EXECUTABLE) $^

run:
	./$(EXECUTABLE)

check:
	./$(EXECUTABLE) > result.txt

clean:
	rm -rf $(EXECUTABLE)

.PHONY: all run check clean
