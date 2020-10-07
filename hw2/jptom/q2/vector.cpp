#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <vector>
#include <cmath>

namespace py = pybind11;

template <typename T1, typename T2>
py::array_t<double> cal_2d_angle(py::array_t<T1> vec1, py::array_t<T2> vec2)
{
	const py::buffer_info & vec1_info = vec1.request();
	const py::buffer_info & vec2_info = vec2.request();

	const ssize_t & vec1_ndim = vec1_info.ndim;
	const ssize_t & vec2_ndim = vec2_info.ndim;
	if ( vec1_ndim != 1 || vec2_ndim != 1 ) { throw std::invalid_argument("invalid input. input vector ndidm must be 1."); }

	const auto &vec1_shape = vec1_info.shape;
	const auto &vec2_shape = vec2_info.shape;
	if ( vec1_shape[0] != 2 || vec2_shape[0] != 2 ) { throw std::invalid_argument("invalid input. input vector shape must be (2,)."); }

	double * data1 = (double *) vec1_info.ptr;
	double * data2 = (double *) vec2_info.ptr;
	if ( (data1[0]==0 && data1[1]==0) || (data2[0]==0 && data2[1]==0) ) { throw std::invalid_argument("invalid input. input vector has zero length."); }

	double dot = data1[0]*data2[0] + data1[1]*data2[1];
	double cross = data1[0]*data2[1] - data1[1]*data2[0];
	double angle = atan2(cross, dot);
	std::cout<<"c++ result: "<<angle<<std::endl;
	py::array_t<double> result({1});
	*result.mutable_data(0) = angle;
	return result;
}

PYBIND11_MODULE(_vector, m)
{
	m.doc() = "hw2 q2 module";
	m.def("cal_2d_angle", &cal_2d_angle<int32_t,int32_t>, "calculate the angle between two vectors");
	m.def("cal_2d_angle", &cal_2d_angle<int32_t,double>, "calculate the angle between two vectors");
	m.def("cal_2d_angle", &cal_2d_angle<double,int32_t>, "calculate the angle between two vectors");
	m.def("cal_2d_angle", &cal_2d_angle<double,double>, "calculate the angle between two vectors");
}
