import _matrix
import numpy as np
import pytest
import timeit

def test_multiply_naive():
    np.random.seed(5555)
    a = np.random.random((1000, 1000))
    b = np.random.random((1000, 1000))

    mat_a = _matrix.Matrix(a)
    mat_b = _matrix.Matrix(b)

    mat_ret = _matrix.multiply_naive(mat_a, mat_b)

    assert mat_ret.nrow == mat_a.nrow
    assert mat_ret.ncol == mat_b.ncol
    assert np.array(mat_ret) == pytest.approx(np.matmul(a, b))

def test_multiply_mkl():
    np.random.seed(5555)
    a = np.random.random((1000, 1000))
    b = np.random.random((1000, 1000))

    mat_a = _matrix.Matrix(a)
    mat_b = _matrix.Matrix(b)

    mat_ret = _matrix.multiply_mkl(mat_a, mat_b)

    assert mat_ret.nrow == mat_a.nrow
    assert mat_ret.ncol == mat_b.ncol
    assert np.array(mat_ret) == pytest.approx(np.matmul(a, b))

def test_multiply_tile():
    np.random.seed(5555)
    a = np.random.random((1000, 1000))
    b = np.random.random((1000, 1000))

    mat_a = _matrix.Matrix(a)
    mat_b = _matrix.Matrix(b)

    mat_ret = _matrix.multiply_tile(mat_a, mat_b, 64)

    assert mat_ret.nrow == mat_a.nrow
    assert mat_ret.ncol == mat_b.ncol
    assert np.array(mat_ret) == pytest.approx(np.matmul(a, b))


def test_performance():
    setup = '''
import _matrix
import numpy as np

np.random.seed(5555)
a = np.random.random((1000, 1000))
b = np.random.random((1000, 1000))
mat_a = _matrix.Matrix(a)
mat_b = _matrix.Matrix(b)
'''

    naive = timeit.Timer("_matrix.multiply_naive(mat_a, mat_b)", setup=setup)
    mkl = timeit.Timer("_matrix.multiply_mkl(mat_a, mat_b)", setup=setup)
    tile = timeit.Timer("_matrix.multiply_tile(mat_a, mat_b, 64)", setup=setup)

    with open("performance.txt", "w") as f:
        f.write(f"Start naive multiply in 5 times, takes min = ")
        naive_sec = min(naive.repeat(repeat=5, number=1))
        f.write(f"{naive_sec} seconds\n")
        f.write(f"Start tile multiply in 5 times, takes min = ")
        tile_sec = min(tile.repeat(repeat=5, number=1))
        f.write(f"{tile_sec} seconds\n")
        f.write(f"Start mkl multiply in 5 times, takes min = ")
        mkl_sec = min(mkl.repeat(repeat=5, number=1))
        f.write(f"{mkl_sec} seconds\n")

        f.write("tile speed-up over naive: %g x\n" % (naive_sec/tile_sec))

