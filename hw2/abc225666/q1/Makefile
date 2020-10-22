main.out: main.cpp
	g++ -o main.out main.cpp

.PHONY: run
run: main.out
	./main.out

.PHONY: check
check: main.out
	./main.out > result.txt

clean:
	rm -rf main.out
