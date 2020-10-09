import _vector as v

def test_two_zero():
 assert v.angle([0, 0], [0, 0]) == -1

def test_zero_angle():
 assert v.angle([1, 0], [1, 0]) == 0

def test_right_angle():
 assert v.angle([1, 0], [0, 1]) == 90

def test_another_angle():
 assert v.angle([1, 1], [0, 1]) == 45