#!/usr/bin/env python

import sys
import os
import timeit
import unittest
import numpy as np
import pytest

# The python module that wraps the matrix code.
import _matrix


class GradingTest(unittest.TestCase):

    def make_matrices(self, size):

        mat1 = _matrix.Matrix(size,size)
        mat2 = _matrix.Matrix(size,size)
        mat3 = _matrix.Matrix(size,size)

        for it in range(size):
            for jt in range(size):
                mat1[it, jt] = it * size + jt + 1
                mat2[it, jt] = it * size + jt + 1
                mat3[it, jt] = 0

        return mat1, mat2, mat3

    def test_basic(self):

        size = 100
        mat1, mat2, mat3, *_ = self.make_matrices(size)

        self.assertEqual(size, mat1.nrow)
        self.assertEqual(size, mat1.ncol)
        self.assertEqual(size, mat2.nrow)
        self.assertEqual(size, mat2.ncol)
        self.assertEqual(size, mat3.nrow)
        self.assertEqual(size, mat3.ncol)

        self.assertEqual(2, mat1[0,1])
        self.assertEqual(size+2, mat1[1,1])
        self.assertEqual(size*2, mat1[1,size-1])
        self.assertEqual(size*size, mat1[size-1,size-1])

        for i in range(mat1.nrow):
            for j in range(mat1.ncol):
                self.assertNotEqual(0, mat1[i,j])
                self.assertEqual(mat1[i,j], mat2[i,j])
                self.assertEqual(0, mat3[i,j])

        self.assertEqual(mat1, mat2)
        self.assertTrue(mat1 is not mat2)
        
    def test_navie(self):
        size = 2
        mat1, mat2, *_ = self.make_matrices(size)

        ret_naive = _matrix.multiply_naive(mat1, mat2)

        self.assertEqual(size, ret_naive.nrow)
        self.assertEqual(size, ret_naive.ncol)