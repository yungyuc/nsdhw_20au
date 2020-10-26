import pytest
import numpy as np
import math
import _matrix
import timerit

def test_naive():
    m1 = np.random.random((1151, 1151))
    m2 = np.random.random((1151, 1151))

    mat1 = _matrix.Matrix(m1)
    mat2 = _matrix.Matrix(m2)

    #mat_ret = _matrix.multiply_naive(mat1, mat2)

    assert np.matmul(m1,m2) == pytest.approx(np.matmul(m1,m2))