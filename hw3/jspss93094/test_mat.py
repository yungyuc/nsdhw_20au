#!/usr/bin/env python3
import _matrix
import unittest
import random
import math
import time

class MatrixTestCase(unittest.TestCase):
	def test_matrix(self):
		size = 1000
		tile_width = random.randint(10, 100)
		mat1 = _matrix.Matrix(size,size)
		mat2 = _matrix.Matrix(size,size)
		for i in range(size):
			for j in range(size):
				mat1[i, j] = random.randint(1, 100)
				mat2[i, j] = random.randint(1, 100)
		starttime = time.time()
		ret_naive = _matrix.multiply_naive(mat1, mat2)
		endtime = time.time()
		naive_time = endtime - starttime
		
		starttime = time.time()
		ret_tile = _matrix.multiply_tile(mat1, mat2, tile_width)
		endtime = time.time()
		tile_time = endtime - starttime
		
		starttime = time.time()
		ret_mkl = _matrix.multiply_naive(mat1, mat2)
		endtime = time.time()
		mkl_time = endtime - starttime
		
		f = open('performance.txt','w')
		f.writelines(['tile_width = ',repr(tile_width),'\nnaive costs ',repr(naive_time),' seconds.\ntile  costs ',repr(tile_time),' seconds.\nmkl   costs ',repr(mkl_time),' seconds.\ntile/naive = ',repr(tile_time/naive_time)])
		f.close()
		for a in range(ret_naive.nrow):
			for b in range(ret_naive.ncol):
				self.assertEqual(ret_naive[a,b], ret_tile[a,b])
				self.assertEqual(ret_tile[a,b], ret_mkl[a,b])
				self.assertEqual(ret_naive[a,b], ret_mkl[a,b])
		self.assertLess(tile_time/naive_time, 0.8)
if __name__ == '__main__':
    unittest.main()
