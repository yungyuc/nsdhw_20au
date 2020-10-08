#include<pybind11/pybind11.h>
#include<iostream>
#include<math.h>

using namespace std;
namespace py = pybind11;

float GetAngle(float v1_x,float v1_y,float v2_x,float v2_y)
{
    float twodot = v1_x*v2_x + v1_y*v2_y;
    float v1_l = sqrt(v1_x*v1_x+v1_y*v1_y);
    float v2_l = sqrt(v2_x*v2_x+v2_y*v2_y);
    if(v1_l == 0 || v2_l == 0) return -1;
    else return acos(twodot/(v1_l*v2_l));
}

PYBIND11_MODULE(_vector, m) {
  m.doc() = "pybind11 example plugin";      // module doc string
  m.def("GetAngle",                              // function name
        &GetAngle,                               // function pointer
        "A function which get angle from two vectors" //function doc string
       );
}
