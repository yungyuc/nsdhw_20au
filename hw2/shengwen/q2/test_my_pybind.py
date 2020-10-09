import _vector
import math

epsilon = math.pi * 0.05 #toleration for float comparsion

print (_vector.angle(1, 1, 1, 1))

def test_my_pybind():
    # Test for zero-length 2-vector (invalid input).
    assert math.fabs(_vector.angle(0, 0, 0, 0) - 0.0) < epsilon

    # Test for zero angle.
    assert math.fabs(_vector.angle(1, 1, 1, 1) - 0.0) < epsilon

    # Test for right angle (90-deg).
    assert math.fabs(_vector.angle(0, 1, 1, 0) - (math.pi / 2)) < epsilon

    # Test for one other angle. (180 degree)
    assert math.fabs(_vector.angle(1, 0, -1, 0) - (math.pi)) < epsilon
