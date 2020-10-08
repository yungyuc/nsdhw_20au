#!/usr/bin/env python3

import _vector
import math

class TestClass:
    def equal_helper(self, v1, v2, deg):
        rst = _vector.cal_angle(v1, v2)
        assert math.isclose(rst, math.radians(deg), rel_tol=1e-6)

    '''Test for zero-length 2-vector (invalid input). '''
    def test_zero_length(self):
        rst = _vector.cal_angle([0, 0], [0, 0])
        assert math.isclose(rst, -1, rel_tol=1e-6)

    '''Test for zero angle.'''
    def test_zero_angle(self):
        self.equal_helper([1, 0], [1, 0], 0)

    '''Test for right angle (90-deg).'''
    def test_right_angle(self):
        self.equal_helper([0, 1], [1, 0], 90)

    '''Test for one other angle.'''
    def test_other_degree(self):
        self.equal_helper([1, 0], [-1, 1], 135)
