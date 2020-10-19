import numpy as np
import math
import time
import pytest

from _matrix import Matrix, multiply_naive, multiply_tile, multiply_mkl

def __exec__(func, tsize):
    #i = np.random.randint(500, 1000)
    #j = np.random.randint(500, 1000)
    #k = np.random.randint(500, 1000)
    i = j = k = 128
    np_mat1 = np.random.random((i, k))
    np_mat2 = np.random.random((k, j))
    mat1 = Matrix(np_mat1)
    mat2 = Matrix(np_mat2)
    if func == multiply_tile:
        result = func(mat1, mat2, tsize)
    else:
        result = func(mat1, mat2)

    assert result.nrow == i
    assert result.ncol == j
    assert np.array(result) == pytest.approx(np.matmul(np_mat1, np_mat2))

def test_multiply_naive():
    __exec__(multiply_naive, 0)

def test_multiply_tile():
    __exec__(multiply_tile, 0)
    __exec__(multiply_tile, 10)

def test_multiply_mkl():
    __exec__(multiply_mkl, 0)

def test_performance():
    naive_timing = []
    for i in range(5):
        start = time.time()
        __exec__(multiply_naive, 0)
        end = time.time()
        naive_timing.append(end - start)

    mkl_timing = []
    for i in range(5):
        start = time.time()
        __exec__(multiply_mkl, 0)
        end = time.time()
        mkl_timing.append(end - start)

    with open('performance.txt', 'w') as f:
        naivesec = np.min(naive_timing)
        mklsec = np.min(mkl_timing)
        print('multiply_naive runtime = {0:2.4f} seconds'.format(naivesec), file=f)
        print('multiply_mkl runtime = {0:2.4f} seconds'.format(mklsec), file=f)
        print('MKL speed-up over naive: {0:2.2f} x'.format(naivesec/mklsec), file=f)
