#!/usr/bin/env python3

import _vector
import math

offset = 1e-6
class TestClass:
    '''Test for zero-length 2-vector (invalid input)'''
    def test_zero_length(self):
        rst = _vector.calculate([0, 0], [0, 0])
        assert math.isclose(rst, -1, rel_tol=offset)

    '''Test for zero angle'''
    def test_zero_vector(self):
        assert math.isclose(_vector.calculate([1, 0], [1, 0]), math.radians(0), rel_tol=offset)

    '''Test for right angle (90-deg)'''
    def test_right_vector(self):
        assert math.isclose(_vector.calculate([0, 1], [1, 0]), math.radians(90), rel_tol=offset)

    '''Test for one other angle'''
    def test_other_degree(self):
        assert math.isclose(_vector.calculate([1, 0], [-1, 1]), math.radians(135), rel_tol=offset)