// Write a C++ function that calculates the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate system.  Use `pybind11 to wrap it to Python. Use Python unit-test to check the result is correct.  The test runner should be py.test.

#include <iostream>
#include <vector>

std::vector<float> str2VecFloat(std::string str);


int main(int, char **)
{

    std::string v1 = "";
    std::string v2 = "";


    std::cout << "Enter first vector (i.e: 0,1): ";
    std::cin >> v1;

    std::cout << "Enter second vector (i.e: 1,0): ";
    std::cin >> v2;


    str2VecFloat(v1);
    str2VecFloat(v2);

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
