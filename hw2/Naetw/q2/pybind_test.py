#!/usr/bin/env python3

from _vector import angle
import math
import numpy as np
import pytest

class TestClass:
    m_abs_tol = 1e-05

    def test_invalid(self):
        vector1 = np.random.randint(64, 1922, 2)
        vector2 = np.array([0, 0])
        vector3 = np.random.randint(64, 1922, 2)

        radian1 = angle(vector1, vector2)
        assert math.isclose(radian1, -127, abs_tol = self.m_abs_tol)

        radian2 = angle(vector2, vector3)
        assert math.isclose(radian2, -127, abs_tol = self.m_abs_tol)

        vector4 = np.array([math.inf, math.inf])
        radian3 = angle(vector1, vector4)
        assert math.isnan(radian3)

    def test_zero_angle(self):
        coefficient = np.random.randint(1, 10)
        vector1 = np.random.randint(64, 1922, 2)
        vector2 = vector1 * coefficient

        radian = angle(vector1, vector2)

        assert math.isclose(radian, 0, abs_tol = self.m_abs_tol)
    
    def test_right_angle(self):
        vector1 = np.array([0, 1])
        vector2 = np.array([1, 0])

        radian = angle(vector1, vector2)

        assert math.isclose(radian, math.pi / 2, abs_tol = self.m_abs_tol)

    def test_180_degree(self):
        vector1 = np.array([1, 0])
        vector2 = np.array([-1, 0])

        radian = angle(vector1, vector2)

        assert math.isclose(radian, math.pi, abs_tol = self.m_abs_tol)

    def test_one_another_degree(self):
        vector1 = np.array([3, 4])
        vector2 = np.array([4, 4])

        radian = angle(vector1, vector2)

        assert math.isclose(radian, (math.pi / 180) * 8.13, abs_tol = self.m_abs_tol)

        coefficient = np.random.randint(64, 1922)
        vector1 = vector1 * coefficient
        vector2 = vector2 * coefficient

        radian = angle(vector1, vector2)

        assert math.isclose(radian, (math.pi / 180) * 8.13, abs_tol = self.m_abs_tol)
