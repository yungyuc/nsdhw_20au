// Write a C++ function that calculates the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate system.  Use `pybind11 to wrap it to Python. Use Python unit-test to check the result is correct.  The test runner should be py.test.

#include <pybind11/pybind11.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <math.h>
#include "_vector.h"


int main(int, char **)
{

    std::string v1_str = "";
    std::string v2_str = "";


    std::cout << "Enter first vector (i.e: 0,1): ";
    std::cin >> v1_str;

    std::cout << "Enter second vector (i.e: 1,0): ";
    std::cin >> v2_str;

    double angle = findAngle2d(v1_str, v2_str);
    std::cout << "Angle: " << angle << std::endl;


    return 0;
}


std::vector<float> str2VecFloat(std::string str)
{

    int c = str.find(",");
    float n1 = std::stof(str.substr(0, c));
    float n2 = std::stof(str.substr(c+1));

    std::vector <float> vec = {n1, n2};

    return vec;
}

double findAngle2d(std::string v1_str, std::string v2_str)
{
    // Convert strings to float
    std::vector<float> v1 = str2VecFloat(v1_str);
    std::vector<float> v2 = str2VecFloat(v2_str);

    // 1. Compute a dot product
    double product = std::inner_product(v1.begin(), v1.end(), v2.begin(), 0);

    // 2. Compute vectors magnitude
    double mag_v1 = sqrt((v1.at(0) * v1.at(0)) + (v1.at(1) * v1.at(1)));
    double mag_v2 = sqrt((v2.at(0) * v2.at(0)) + (v2.at(1) * v2.at(1)));

    // 3. Compute the angle
    double angle = product / (mag_v1 * mag_v2);

    /* return product; */
    return angle;
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "pybind11 wrapper for calcAngle";
    m.def("findAngle2d", &findAngle2d, "Compute angle between two vectors");
}
