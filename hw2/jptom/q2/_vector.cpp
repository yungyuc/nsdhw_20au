#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <cmath>

namespace py = pybind11;

py::array_t<double> cal_2d_angle(py::array_t<double> vec1, py::array_t<double> vec2)
{
	py::buffer_info vec1_info = vec1.request();
	py::buffer_info vec2_info = vec2.request();

	if ( vec1_info.ndim != 1 || vec2_info.ndim != 1 ) { throw std::invalid_argument("invalid input. input vector ndidm must be 1."); }

	if ( vec1_info.size != 2 || vec2_info.size != 2 ) { throw std::invalid_argument("invalid input. input vector size must be 2."); }

	double * data1 = (double *) vec1_info.ptr;
	double * data2 = (double *) vec2_info.ptr;
	if ( (data1[0]==0 && data1[1]==0) || (data2[0]==0 && data2[1]==0) ) { throw std::invalid_argument("invalid input. input vector has zero length."); }
	//
	//std::cout<<"vec1:[ "<<data1[0]<<", "<<data1[1]<<"]"<<std::endl;
	//std::cout<<"vec2:[ "<<data2[0]<<", "<<data2[1]<<"]"<<std::endl;
	//
	//std::cout<<"vec1:[ "<<*vec1.data(0)<<", "<<*vec1.data(1)<<"]"<<std::endl;
	//std::cout<<"vec2:[ "<<*vec2.data(0)<<", "<<*vec2.data(1)<<"]"<<std::endl;
	//
	double dot = data1[0]*data2[0] + data1[1]*data2[1];
	double cross = data1[1]*data2[0] - data1[0]*data2[1];
	double angle = atan2(cross, dot);
	//
	//std::cout<<"c++ result: "<<angle<<std::endl;
	//
	py::array_t<double> result({1});
	*result.mutable_data(0) = angle;
	return result;
}

PYBIND11_MODULE(_vector, m)
{
	m.doc() = "hw2 q2 module";
	m.def("cal_2d_angle", &cal_2d_angle, "calculate the angle between two vectors");
}
