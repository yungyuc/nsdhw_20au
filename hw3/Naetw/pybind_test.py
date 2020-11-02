#!/usr/bin/env python3

from _matrix import Matrix, multiply_naive, multiply_tile, multiply_mkl
import math
import numpy as np
import pytest
import timeit

class TestClass:
    def test_multiply_naive(self):
        for i in range(2):
            row_n_col = 1000
            np_mat1 = np.random.random(row_n_col * row_n_col)
            np_mat2 = np.random.random(row_n_col * row_n_col)
            mat1 = Matrix(row_n_col, row_n_col, np_mat1.tolist())
            mat2 = Matrix(row_n_col, row_n_col, np_mat2.tolist())

            np_mat1 = np_mat1.reshape((row_n_col, row_n_col))
            np_mat2 = np_mat2.reshape((row_n_col, row_n_col))

            naive_answer = multiply_naive(mat1, mat2)

            assert naive_answer.nrow == row_n_col
            assert naive_answer.ncol == row_n_col

            mkl_answer = multiply_mkl(mat1, mat2)

            naive_ver_in_np = np.array(naive_answer.buffer_vector()).reshape((row_n_col, row_n_col))
            mkl_ver_in_np = np.array(mkl_answer.buffer_vector()).reshape((row_n_col, row_n_col))
            assert naive_ver_in_np == pytest.approx(mkl_ver_in_np, abs=1e-05)

    def test_multiply_tile(self):
        def check_tile(row, col1, col2, tile_size):
            np_mat1 = np.random.random(row * col1)
            np_mat2 = np.random.random(col1 * col2)
            mat1 = Matrix(row, col1, np_mat1.tolist())
            mat2 = Matrix(col1, col2, np_mat2.tolist())

            np_mat1 = np_mat1.reshape((row, col1))
            np_mat2 = np_mat2.reshape((col1, col2))

            tile_answer = multiply_tile(mat1, mat2, tile_size)

            assert tile_answer.nrow == row
            assert tile_answer.ncol == col2

            mkl_answer = multiply_mkl(mat1, mat2)

            tile_ver_in_np = np.array(tile_answer.buffer_vector()).reshape((row, col2))
            mkl_ver_in_np = np.array(mkl_answer.buffer_vector()).reshape((row, col2))
            assert tile_ver_in_np == pytest.approx(mkl_ver_in_np, abs=1e-05)

            # benchmark
            ns = dict(multiply_naive=multiply_naive, multiply_tile=multiply_tile,\
                      mat1=mat1, mat2=mat2, tile_size=tile_size)
            t_naive = timeit.Timer('multiply_naive(mat1, mat2)', globals=ns)
            t_tile = timeit.Timer('multiply_tile(mat1, mat2, tile_size)', globals=ns)
            # t_mkl = timeit.Timer('_matrix.multiply_mkl(mat1, mat2)', globals=ns)

            time_naive = min(t_naive.repeat(5, 1))
            time_tile = min(t_tile.repeat(5, 1))
            # time_mkl = min(t_mkl.repeat(10, 1))
            ratio = time_tile/time_naive

            with open("performance.txt", "w") as file:
                file.write("tile time (tile_size: {}) / naive time: {}/{}={}".format(tile_size, time_tile, time_naive, ratio))
            return ratio

        row = np.random.randint(1000, 1050)
        col1 = np.random.randint(1000, 1050)
        col2 = np.random.randint(1000, 1050)
        ratio0 = check_tile(row, col1, col2, 0)
        ratio16 = check_tile(row, col1, col2, 16)
        ratio17 = check_tile(row, col1, col2, 17)
        ratio19 = check_tile(row, col1, col2, 19)

        assert ratio16 < 0.8
        assert ratio17 < 0.8
        assert ratio19 < 0.8

    def test_multiply_mkl(self):
        for i in range(2):
            row_n_col = 1000
            np_mat1 = np.random.random(row_n_col * row_n_col)
            np_mat2 = np.random.random(row_n_col * row_n_col)
            mat1 = Matrix(row_n_col, row_n_col, np_mat1.tolist())
            mat2 = Matrix(row_n_col, row_n_col, np_mat2.tolist())

            np_mat1 = np_mat1.reshape((row_n_col, row_n_col))
            np_mat2 = np_mat2.reshape((row_n_col, row_n_col))

            answer = multiply_mkl(mat1, mat2)

            assert answer.nrow == row_n_col
            assert answer.ncol == row_n_col

            assert np.array(answer.buffer_vector()).reshape((row_n_col, row_n_col)) ==\
                   pytest.approx(np.matmul(np_mat1, np_mat2))
