// Line class definitions and functions
#pragma once
#include <cstddef>

class Line
{
    public:
        Line();
        Line(size_t size);
        ~Line();
        size_t size() const;
        float const & x(size_t i) const; // getter
        float       & x(size_t i);       // setter

        float const & y(size_t i) const; // getter
        float       & y(size_t i);       // setter

    private:
        // Member data.
        size_t m_size = 0;
        float * m_elements_x;
        float * m_elements_y;
};

