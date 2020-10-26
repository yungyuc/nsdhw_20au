import pytest
import numpy as np
import math
from _matrix import Matrix, multiply_naive, multiply_tile, multiply_mkl

def test_naive():
    m = np.random.randint(600, 1151)
    n = np.random.randint(600, 1151)
    k = np.random.randint(600, 1151)

    np_mat1 = np.random.random((m, k))
    np_mat2 = np.random.random((k, n))

    mat1 = Matrix(np_mat1)
    mat2 = Matrix(np_mat2)

    mat_ret = multiply_naive(mat1, mat2)

    assert np.array(mat_ret) == pytest.approx(np.matmul(np_mat1,np_mat2))