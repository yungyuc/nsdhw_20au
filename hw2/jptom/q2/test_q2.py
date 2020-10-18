#!/usr/bin/env python
import _vector
import numpy as np
import numpy.testing as npt

def rotate_2d(vec, theta):
    t_mat = np.array([[np.cos(theta),   np.sin(theta)],
                      [-np.sin(theta), np.cos(theta)]])
    return np.dot(t_mat, vec)

def test_zero_vector():
    vec = np.random.randn(2)
    zero_vec = np.zeros(2)
    npt.assert_raises(ValueError, _vector.cal_2d_angle, vec, zero_vec)
    npt.assert_raises(ValueError, _vector.cal_2d_angle, zero_vec, vec)
    npt.assert_raises(ValueError, _vector.cal_2d_angle, zero_vec, zero_vec)

def test_angle_0():
    vec1 = np.random.randn(2)
    vec2 = vec1
    angle = _vector.cal_2d_angle(vec1, vec2)
    npt.assert_almost_equal(angle, 0/180*np.pi)
    angle = _vector.cal_2d_angle(vec1, vec2*2)
    npt.assert_almost_equal(angle, 0/180*np.pi)
    angle = _vector.cal_2d_angle(vec2*2, vec1)
    npt.assert_almost_equal(angle, 0/180*np.pi)

def test_angle_90():
    vec1 = np.random.randn(2)
    vec2 = rotate_2d(vec1, 90/180*np.pi)
    angle = _vector.cal_2d_angle(vec1, vec2)
    npt.assert_almost_equal(angle, 90/180*np.pi)
    angle = _vector.cal_2d_angle(vec2, vec1)
    npt.assert_almost_equal(angle, -90/180*np.pi)

def test_angle_180():
    vec1 = np.random.randn(2)
    vec2 = rotate_2d(vec1, 180/180*np.pi)
    angle = _vector.cal_2d_angle(vec1, vec2)
    npt.assert_almost_equal(angle, 180/180*np.pi)
    angle = _vector.cal_2d_angle(vec2, vec1)
    npt.assert_almost_equal(angle, -180/180*np.pi)

def test_angle_random():
    vec1 = np.random.randn(2)
    random_angle = np.random.rand()*180
    vec2 = rotate_2d(vec1, random_angle/180*np.pi)
    angle = _vector.cal_2d_angle(vec1, vec2)
    npt.assert_almost_equal(angle, random_angle/180*np.pi)
    angle = _vector.cal_2d_angle(vec2, vec1)
    npt.assert_almost_equal(angle, -random_angle/180*np.pi)

