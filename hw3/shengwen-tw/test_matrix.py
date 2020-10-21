import _matrix
import pytest
import timeit
import math

#run this with `python3 test_matrix.py` for displaying debug message

def matrix_compare(mat1, mat2):
    assert mat1.nrow == mat2.nrow
    assert mat1.ncol == mat2.ncol

    epsilon = 1e-6 #float comparsion tolerance

    error_cnt = 0

    for i in range(0, mat1.nrow):
        for j in range(0, mat2.ncol):
            abs_diff = math.fabs(mat1[i, j] - mat2[i, j])
            if abs_diff > 1e-6:
                error_cnt = error_cnt + 1
    
    assert error_cnt <= 0

def eval_items(N, block_size):
    print('test matrix size: %dx%d, block size=%d' %(N, N, block_size))

    mat1 = _matrix.Matrix(N, N)
    _matrix.random_matrix(mat1, 1000)

    mat2 = _matrix.Matrix(N, N)
    _matrix.random_matrix(mat2, 1000)

    result_naive = _matrix.Matrix(N, N)
    result_tile = _matrix.Matrix(N, N)
    result_mkl = _matrix.Matrix(N, N)

    result_naive = _matrix.multiply_naive(mat1, mat2)

    result_tile = _matrix.multiply_tile(mat1, mat2, block_size)

    result_mkl = _matrix.multiply_mkl(mat1, mat2)

    #elementwise comparision test written in python
    matrix_compare(result_naive, result_mkl)
    matrix_compare(result_tile, result_mkl)
    matrix_compare(result_naive, result_tile)

    #test with overloaded == C++ object operator
    assert result_naive == result_mkl
    assert result_tile == result_mkl
    assert result_naive == result_tile

def test_me():
    #test multiply with block size 1~64 on 100x100 dimension
    #random matricies
    eval_items(100, 1)
    eval_items(100, 2)
    eval_items(100, 4)
    eval_items(100, 8)
    eval_items(100, 16)
    eval_items(100, 32)
    eval_items(100, 64)

    #test multiply with block size 1~64 on 333x333 dimension
    #random matricies
    eval_items(333, 1)
    eval_items(333, 2)
    eval_items(333, 4)
    eval_items(333, 8)
    eval_items(333, 16)
    eval_items(333, 32)
    eval_items(333, 64)

    #test multiply with block size 1~64 on 500x500 dimension
    #random matricies
    eval_items(500, 64) #too slow to test all, let's just test one here

    #test multiply with block size 1~64 on 1000x1000 dimension
    #random matricies
    eval_items(1024, 64) #too slow to test all, let's just test one here

    #performance testing
    mat1 = _matrix.Matrix(1024, 1024)
    _matrix.random_matrix(mat1, 10000)

    mat2 = _matrix.Matrix(1024, 1024)
    _matrix.random_matrix(mat2, 10000)

    result_naive = _matrix.Matrix(1024, 1024)
    result_tile = _matrix.Matrix(1024, 1024)
    result_mkl = _matrix.Matrix(1024, 1024)

test_me()
