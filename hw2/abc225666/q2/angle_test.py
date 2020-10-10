import _vector
import math
import pytest

def test_45():
    # 45 degree
    assert math.isclose(_vector.angle(0, 100, 5, 5),  math.pi/4)

def test_30():
    # 30 degree
    assert math.isclose(_vector.angle(math.sqrt(3), 1, 1, math.sqrt(3)), math.pi/6)

def test_0_vector():
    assert math.isnan(_vector.angle(0, 0, 1, 200))

def test_90():
    assert math.isclose(_vector.angle(1000, 1000, -2000, 2000), math.pi/2)
