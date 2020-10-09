#include <iostream>
#include <vector>

class Line
{
public:
    Line() {}
    Line(Line const & other) : m_size(other.m_size), m_coord(other.m_coord) {} 
    Line(Line       && other) : m_size(std::move(other.m_size)), m_coord(std::move(other.m_coord)) {}
    Line & operator=(Line const & other) {
        if (this == &other) { return *this; }
        m_coord = other.m_coord;
        m_size = other.m_size;
        
        return *this;
    }
    Line & operator=(Line       && other) {
        if (this == &other) { return *this; } 
        std::swap(other.m_size, m_size);
        m_coord.swap(other.m_coord);
        return *this;
    }
    Line(size_t size) { m_size = size; m_coord.resize(size*2, 0); }
    ~Line() {}
    size_t size() const {return m_size;}
    const float & x(size_t it) const {return m_coord.at(it*2);}
    float & x(size_t it) {return m_coord.at(it*2);}
    const float & y(size_t it) const {return m_coord.at(it*2+1);}
    float & y(size_t it) {return m_coord.at(it*2+1);}
private:
    size_t m_size = 0; 
    std::vector<float> m_coord;
}; /* end class Line */

int main(int, char **)
{
    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}