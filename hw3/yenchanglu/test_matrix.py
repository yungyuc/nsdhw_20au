import pytest
import numpy as np
import math
import _matrix

def test_naive():
    m1 = np.random.random((1151, 1151))
    m2 = np.random.random((1151, 1151))

    mat1 = _matrix.Matrix(m1)
    mat2 = _matrix.Matrix(m2)

    mat_ret = _matrix.multiply_naive(mat1, mat2)

    assert np.array(mat_ret) == pytest.approx(np.matmul(m1,m2))