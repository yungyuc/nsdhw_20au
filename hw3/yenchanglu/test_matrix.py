import pytest
import unittest
import numpy as np
import math
import time
from _matrix import Matrix, multiply_naive, multiply_tile, multiply_mkl

class test_matrix(unittest.TestCase):
    def test_result(self):
        mat1 = Matrix(np.random.random((1151,1151)))
        mat2 = Matrix(np.random.random((1151,1151)))

        naive_ret = multiply_naive(mat1, mat2)
        tile_ret = multiply_tile(mat1, mat2, 64)
        mkl_ret = multiply_mkl(mat1, mat2)
        for i in range(naive_ret.nrow):
            for j in range(naive_ret.ncol):
                assert naive_ret[i,j] == pytest.approx(mkl_ret[i,j], abs=1e-05)
                assert tile_ret[i,j] == pytest.approx(mkl_ret[i,j], abs=1e-05)

    def test_performance(self):
        mat1 = Matrix(np.random.random((1151,1151)))
        mat2 = Matrix(np.random.random((1151,1151)))

        timer_naive = []
        timer_tile = []
        timer_mkl = []

        for i in range(5):
            timer = time.time()
            multiply_naive(mat1, mat2)
            timer_naive.append(time.time() - timer)

            timer = time.time()
            multiply_tile(mat1, mat2, 64)
            timer_tile.append(time.time() - timer)

            timer = time.time()
            multiply_mkl(mat1, mat2)
            timer_mkl.append(time.time() - timer)

        naive_avg = np.average(timer_naive)
        tile_avg = np.average(timer_tile)
        mkl_avg = np.average(timer_mkl)

        with open("performance.txt", "w") as f:
            f.write("[Average execution time]\n")
            f.write("multiply_naive: {} secs\n".format("%.4f"%naive_avg))
            f.write("multiply_tile : {} secs\n".format("%.4f"%tile_avg))
            f.write("multiply_mkl: {} secs\n".formate("%.4f"%mkl_avg))
            f.write("The tiling version is {:.1%} faster than naive version\n".format(naive_avg / tile_avg))
            f.write("The mkl version is {:.1%} faster than naive version\n".format(naive_avg / tile_avg))
            f.close()

if __name__ == "__main__":
    unittest.main()