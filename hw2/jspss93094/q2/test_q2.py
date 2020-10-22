#!/usr/bin/env python
import _vector
import unittest
import random
import math

class VectorTestCase(unittest.TestCase):
    def test_zero_length(self):
        expected = -1;
        v1_x = random.uniform(1,1000);
        v1_y = random.uniform(1,1000);
        v2_x = 0;
        v2_y = 0;
        result = _vector.GetAngle(v1_x,v1_y,v2_x,v2_y);
        self.assertEqual(expected, result);
    def test_zero_angle(self):
        expected = 0;
        v1_x = random.uniform(1,1000);
        v1_y = 0;
        v2_x = random.uniform(1,1000);
        v2_y = 0;
        result = _vector.GetAngle(v1_x,v1_y,v2_x,v2_y);
        self.assertEqual(expected, result);
    def test_right_angle(self):
        expected = 1.57080;
        v1_x = random.uniform(1,1000);
        v1_y = 0;
        v2_x = 0;
        v2_y = random.uniform(1,1000);
        result = round(_vector.GetAngle(v1_x,v1_y,v2_x,v2_y),5);
        self.assertEqual(expected, result);
    def test_45_angle(self):
        expected = 0.78540;
        v1_x = random.uniform(1,1000);
        v1_y = 0;
        v2_x = v1_x;
        v2_y = v1_x;
        result = round(_vector.GetAngle(v1_x,v1_y,v2_x,v2_y),5);
        self.assertEqual(expected, result);
if __name__ == '__main__':
    unittest.main()
