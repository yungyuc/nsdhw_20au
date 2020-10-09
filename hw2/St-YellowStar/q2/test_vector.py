import _vector

import math

def test_zero_length():
    assert abs(_vector.calcAngle([0,0],[1,2]) + 1.0) < 0.0001

def test_nighty_degree():
    assert abs(_vector.calcAngle([1,0],[0,1]) - math.pi/2) < 0.0001

def test_zero_degree():
    assert abs(_vector.calcAngle([1,0],[1,0]) - 0.0) < 0.0001
