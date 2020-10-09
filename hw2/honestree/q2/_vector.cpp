#pragma once

#include <cstdint>
#include <iostream>
#include <cmath>
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(example, m) {
    m.def("add", &add, "A function which adds two numbers");
}



struct Point{
    float x = 0;
    float y = 0;
};

class _vector{

    public:

        // Basic constructors.
        _vector() = default; // default constructor.
        _vector(_vector const & ); // copy constructor.
        _vector(_vector       &&); // move constructor.
        _vector & operator=(_vector const & ); // copy assignment operator.
        _vector & operator=(_vector       &&); // move assignment operator.

        // Custom constructor.
        _vector( float , float , float , float);

        // Desctructor.
        ~_vector() {}

        // Accessors.
        float Length();
        float Angle( _vector );
        float Inner_prod( _vector );

    private:

        Point startP;
        Point endP;

};



float _vector::Angle( _vector other ){

    if( this->Length() * other.Length() != 0 ){
        return acos( this->Inner_prod( other )/( this->Length() * other.Length() )) ;
    }
    else{
        return std::nan("");

    }

}

float _vector::Length(){

    return std::sqrt( std::pow(( endP.x - startP.x ), 2.0)+ std::pow(( endP.y - startP.y ), 2.0));
}

float _vector::Inner_prod( _vector other ){

    return (( endP.x - startP.x ) * ( other.endP.x - other.startP.x ))+(( endP.y - startP.y ) * ( other.endP.y - other.startP.y ));

}



// Define the custom constructor
_vector::_vector( float startx, float starty, float endx, float endy ){

    startP.x = startx;
    startP.y = starty;
    endP.x = endx;
    endP.y = endy;
}

// Define the copy constructor
_vector::_vector(_vector const & other){
    startP = other.startP;
    endP = other.endP;
}

// Define the move constructor
_vector::_vector(_vector && other)
{
    std::swap(other.startP, startP);
    std::swap(other.endP, endP);
}

// Define the copy assignment operator
_vector & _vector::operator=(_vector const & other)
{
    if (this == &other) { return *this; } // don't move to self.
    // This part is the same as what's in the copy constructor.
    startP = other.startP;
    endP = other.endP;
    return *this;
}

// Define the move assignment operator
_vector & _vector::operator=(_vector && other)
{
    if (this == &other) { return *this; } // don't move to self.
    std::swap(other.startP, startP);
    std::swap(other.endP, endP);
    return *this;
}

