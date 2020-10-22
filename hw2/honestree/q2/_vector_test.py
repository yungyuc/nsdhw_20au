from _vector import Angle
import math
import numpy as np
import pytest

def test_correctness():
    assert math.isclose( Angle(  0.0 , 0.0 , 1.1, 1.1 , 0.0 , 0.0 , 9.0, 0.0 ), math.pi/4 , abs_tol = 1e-06)
def test_zero_angle():
    assert math.isclose( Angle(  0.0 , 0.0 , 1.1, 1.1 , 0.0 , 0.0 , 1, 1 ), 0 , abs_tol = 1e-06)
    assert math.isclose( Angle(  0.0 , 0.0 , 1.1, 1.1 , 2 , 2 , 9, 9 ), 0 , abs_tol = 1e-06)
def test_ortho():
    assert math.isclose( Angle(  2 , 2 , -8 , 12 , 0 , 0 , 1, 1), math.pi/2 , abs_tol = 1e-06)
    assert math.isclose( Angle(  0 , 0 , 69 , 0 , 0 , 0 , 0, 169), math.pi/2 , abs_tol = 1e-06)
def test_nan():
    assert math.isnan( Angle(  0 , 0 , 69 , 0 , 9 , 9 , 9, 9) )
