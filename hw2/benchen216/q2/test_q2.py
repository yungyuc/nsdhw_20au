import _vector


def test_zero_angle():
    tem = _vector.angle((1, 1), (1, 1))
    assert int(tem) == 0


def test_right_angle():
    tem = _vector.angle((0, 1), (1, 0))
    assert int(tem) == 90


def test_angle45():
    tem = _vector.angle((0, 1), (1, 1))
    assert int(tem) == 45