// Implementation of Line class with STL containers
#include "Line.h"
#include <vector>

Line::Line() {}

Line::Line(size_t size) : m_size(size)
{
    m_elements_x = std::vector<float>(size);
    m_elements_y = std::vector<float>(size);
}

Line::~Line() { }

extern size_t Line::size() const { return m_size; }

// getter 
extern float const & Line::x(size_t i) const { return m_elements_x.at(i); }
// setter
float & Line::x(size_t i) { return m_elements_x.at(i); }

// getter 
extern float const & Line::y(size_t i) const { return m_elements_y.at(i); }
// setter
float & Line::y(size_t i) { return m_elements_y.at(i); }
