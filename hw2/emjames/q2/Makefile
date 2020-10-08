# running ``make`` should build your code.  ``make test`` runs the Python-based
# tests.  ``make clean`` removes all the built and intermediate files.
#
CXX = g++

calcAngle: calcAngle.o
	$(CXX) calcAngle.o -o calcAngle

calcAngle.o: calcAngle.cpp
	$(CXX) -c calcAngle.cpp -o calcAngle.o

.PHONY:
clean:
	rm -rf calcAngle *.o

