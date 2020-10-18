#!/usr/bin/env python3

import pytest
import math
import numpy as np
import _vector

def angle(vec1, vec2):
    dot_product = np.dot(vec1, vec2)
    len1 = np.dot(vec1, vec1)
    len2 = np.dot(vec2, vec2)
    return np.arccos(dot_product / (np.sqrt(len1 * len2)))

def testInvalidVector():
    with pytest.raises(TypeError):
        _vector.angle([0, 0], [])
    with pytest.raises(TypeError):
        _vector.angle([], [0, 0])
    with pytest.raises(TypeError):
        _vector.angle([], [])
    with pytest.raises(TypeError):
        _vector.angle(["1", 2], [1, 1])

# Test for zero-length 2-vector (invalid input).
def testZeroLengthVector():
    with pytest.raises(ValueError):
        _vector.angle([1, 1], [0, 0])
    with pytest.raises(ValueError):
        _vector.angle([0, 0], [1, 1])
    with pytest.raises(ValueError):
        _vector.angle([0, 0], [0, 0])

# Test for zero angle.
def testZeroAngle():
    for i in range(10):
        mult = np.random.randint(1, 10)
        vec1 = np.random.randint(1, 10, 2)
        vec2 = vec1 * mult
        assert (_vector.angle(vec1, vec2)) == 0

# Test for right angle (90-deg) and straight angle (180-degree).
def testRightAndStraightAngle():
    for i in range(10):
        vec1 = np.random.randint(1, 10, 2)
        vec2 = np.array([-vec1[1], vec1[0]])
        vec3 = np.array([vec1[1], -vec1[0]])
        # Test for 90-degree.
        pytest.approx(_vector.angle(vec1, vec2), math.pi / 2)
        pytest.approx(_vector.angle(vec1, vec3), math.pi / 2)
        # Test for 180-degree.
        pytest.approx(_vector.angle(vec2, vec3), math.pi)

# Test for one other angle.
def testArbitraryVector():
    for i in range(10):
        vec1 = np.random.randint(1, 10, 2)
        vec2 = np.random.randint(1, 10, 2)
        pytest.approx(_vector.angle(vec1, vec2), angle(vec1, vec2))

def testAngle():
    for i in range(10):
        theta = 1 / np.random.randint(1, 10)
        pytest.approx(theta, _vector.angle([np.cos(theta), np.sin(theta)], [1, 0]))
        pytest.approx(theta, _vector.angle([np.cos(theta), np.sin(-theta)], [1, 0]))
