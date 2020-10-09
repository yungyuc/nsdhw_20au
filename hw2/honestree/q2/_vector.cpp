#pragma once

#include <cstdint>
#include <iostream>
#include <cmath>
#include <pybind11/pybind11.h>

namespace py = pybind11;


float Angle( float aStartX, float aStartY, float aEndX, float aEndY,
 			 float bStartX, float bStartY, float bEndX, float bEndY){

	float aLeng = std::sqrt( std::pow(( aEndX - aStartX ), 2.0)+ std::pow(( aEndY - aStartY ), 2.0));
	float bLeng = std::sqrt( std::pow(( bEndX - bStartX ), 2.0)+ std::pow(( bEndY - bStartY ), 2.0));

	if( aLeng * bLeng == 0 ){
		return std::nan("");
	}
	else{
		return acos( ((aEndX - aStartX) * (bEndX - bStartX)+(aEndY - aStartY)*(bEndY - bStartY)) / ( aLeng * bLeng ));
	}
}

PYBIND11_MODULE( _vector, m ) {
  m.doc() = "_vector module";
  m.def("Angle", &Angle, "Angle calculator");
}
