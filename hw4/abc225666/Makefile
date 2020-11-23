TARGET = _matrix$(shell python3-config --extension-suffix)

#MKLROOT = /opt/intel/mkl/lib/intel64
#MKLINCLUDEROOT = /opt/intel/mkl

MKLROOT = ${HOME}/opt/conda/lib
MKLINCLUDEROOT = ${HOME}/opt/conda

MKLLINE := \
	-Wl,--no-as-needed \
	-I${MKLINCLUDEROOT}/include \
	${MKLROOT}/libmkl_intel_lp64.so \
	${MKLROOT}/libmkl_sequential.so \
	${MKLROOT}/libmkl_core.so \
	${MKLROOT}/libmkl_avx2.so \
	${MKLROOT}/libmkl_def.so \
	-lpthread \
	-lm \
	-ldl

$(TARGET): _matrix.cpp allocator.cpp
	g++ -O3 -Wall -shared -std=c++17 -fPIC `python3 -m pybind11 --includes` _matrix.cpp allocator.cpp -o _matrix`python3-config --extension-suffix` ${MKLLINE}

test: $(TARGET)
	pytest

clean:
	rm -rf $(TARGET) __pycache__ .pytest_cache performance.txt
