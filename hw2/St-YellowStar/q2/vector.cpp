#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <iostream>
#include <cmath>
#include <vector>

namespace py = pybind11;

float calcAngle(std::vector<float> a, std::vector<float> b){	
	float dot, mag_a, mag_b;
	dot = a.at(0) * b.at(0) + a.at(1) * b.at(1);
	mag_a = a.at(0) * a.at(0) + a.at(1) * a.at(1);
	mag_b = b.at(0) * b.at(0) + b.at(1) * b.at(1);
	if(mag_a < 0.0001f || mag_b < 0.0001f ){
		std::cout << "invalid input\n";
		return -1.0f;
	}
	return acos(dot / sqrt(mag_a) * sqrt(mag_b));
}

PYBIND11_MODULE(_vector, m){
	m.doc() = "pybind11 vector calculation";

	m.def("calcAngle", &calcAngle, "A function calculate angle between two vectors" );

}

