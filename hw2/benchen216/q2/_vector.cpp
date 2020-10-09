#include <pybind11/pybind11.h>
#include <cmath>
#include <tuple>
#define PI 3.141592653589793
double angle(
    std::tuple<double, double> const & vec,
    std::tuple<double, double> const & vec1
)
{
double ab,a1,b1,cosr;
ab=std::get<0>(vec)*std::get<0>(vec1)+std::get<1>(vec)*std::get<1>(vec1);
a1=sqrt(std::get<0>(vec)*std::get<0>(vec)+std::get<1>(vec)*std::get<1>(vec));
b1=sqrt(std::get<0>(vec1)*std::get<0>(vec1)+std::get<1>(vec1)*std::get<1>(vec1));
cosr=ab/a1/b1;
return acos(cosr)*180/PI;

}
PYBIND11_MODULE(_vector, mod)
{

    mod.doc() = "example C extension module";
    mod.def("angle", &angle, "vector angle");

}