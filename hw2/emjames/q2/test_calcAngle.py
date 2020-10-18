import _vector 
import pytest

def test_angle1():
    angle = _vector.findAngle2d("3,4", "4,3")
    assert angle == 0.96

def test_angle2():
    angle = _vector.findAngle2d("7,1", "5,5")
    assert round(angle, 2) == 0.80

