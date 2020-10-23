import _matrix as m
import random
import numpy as np

size = 11

def test_insert():
    
    mat1 = m.Matrix(size, size)
    mat1[ 10,10 ] = 100
    
    assert mat1[ 10,10 ] == 100

def test_copy():
    
    mat1 = m.Matrix(size, size)

    for x in range( size ):
        for y in range( size ):
            mat1[x,y] = x*y
    
    mat2 = mat1
    
    assert mat2[ 10,10 ] == 100

def test_compare():
    
    mat1 = m.Matrix(size, size)

    for x in range( size ):
        for y in range( size ):
            mat1[x,y] = x*y
    
    mat2 = mat1
    
    assert mat1 == mat2
    
def test_naive():
    
    mat1 = m.Matrix(size, size)
    mat2 = m.Matrix(size, size)

    for x in range( size ):
        for y in range( size ):
            mat1[x,y] = random.random()
            mat2[x,y] = random.random()

    naive = m.multiply_naive(mat1, mat2)
    tile = m.multiply_tile(mat1, mat2)
    mkl = m.multiply_mkl(mat1, mat2)
    
    assert mkl == naive
    assert mkl == tile
