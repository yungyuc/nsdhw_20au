import _vector
import math
import pytest

def test_zero_length():
    assert math.isnan(_vector.angle(0, 0, 0, 0))

def test_0_angle():
    assert math.isclose(_vector.angle(300, 600, 15, 30), 0)

def test_right_angle():
    assert math.isclose(_vector.angle(1000, 1000, -2000, 2000), math.pi/2)

def test_45_angle():
    # 45 degree
    assert math.isclose(_vector.angle(0, 100, 5, 5),  math.pi/4)

def test_30_angle():
    # 30 degree
    assert math.isclose(_vector.angle(math.sqrt(3), 1, 1, math.sqrt(3)), math.pi/6)


