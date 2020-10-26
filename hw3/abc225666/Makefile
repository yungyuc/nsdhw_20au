TARGET = _matrix$(shell python3-config --extension-suffix)

MKLROOT = ${HOME}/opt/conda

MKLLINE := \
	-I${MKLROOT}/include \
	-lpthread \
	-lm \
	-ldl \
	-shared \
	${MKLROOT}/lib/libmkl_intel_lp64.so \
	${MKLROOT}/lib/libmkl_sequential.so \
	${MKLROOT}/lib/libmkl_core.so \
	${MKLROOT}/lib/libmkl_avx2.so \
	${MKLROOT}/lib/libmkl_def.so \
	-Wl,--no-as-neeeded

$(TARGET): _matrix.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _matrix.cpp -o _matrix`python3-config --extension-suffix` ${MKLLINE}

test: $(TARGET)
	pytest

clean:
	rm -rf $(TARGET) __pycache__ .pytest_cache performance.txt
