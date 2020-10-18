// Line class definitions and functions
#pragma once
#include <cstddef>
#include <vector>

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
        std::vector<float> m_elements_x;
        std::vector<float> m_elements_y;
};

