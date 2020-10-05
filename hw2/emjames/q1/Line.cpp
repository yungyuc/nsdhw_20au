// Implementation of Line class with pointers
#include "Line.h"

Line::Line() {}

Line::Line(size_t size) : m_size(size)
{
    m_elements_x = new float[size]; 
    m_elements_y = new float[size]; 
}

Line::~Line() { }

extern size_t Line::size() const { return m_size; }

// getter 
extern float const & Line::x(size_t i) const { return m_elements_x[i]; } 
// setter
float & Line::x(size_t i) { return m_elements_x[i]; }

// getter 
extern float const & Line::y(size_t i) const { return m_elements_y[i]; } 
// setter
float & Line::y(size_t i) { return m_elements_y[i]; }
