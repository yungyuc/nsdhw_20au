#ifndef  LINE_HPP
#define  LINE_HPP
#include <vector>
class Line
{
    public:
        Line();
        Line(Line const & other);
        Line(Line       && other);
        Line & operator=(Line const & other);
        Line & operator=(Line       && other);
        Line(size_t size) : m_size(size), m_coord(2 * size) {}
        ~Line() {}
        size_t size() const {return  m_size; }
        const float & x(size_t it) const { return m_coord.at(it*2); }
        float & x(size_t it) { return m_coord.at(it*2); }
        const float & y(size_t it) const { return m_coord.at(it*2+1); }
        float & y(size_t it) { return m_coord.at(it*2+1); }
    private:
        size_t m_size = 0;

        std::vector<float> m_coord;
};

Line::Line(Line const & other) {
    m_size = other.m_size;
    m_coord = other.m_coord;
}

Line::Line(Line && other) {
    m_size = other.m_size;
    m_coord = move(other.m_coord);
}

Line & Line::operator=(Line const &other) {
    if(this == &other) {
        return *this;
    }
    m_size = other.m_size;
    m_coord = other.m_coord;
    return *this;
}

Line & Line::operator=(Line && other) {
    if(this == &other) {
        return *this;
    }
    m_size = other.m_size;
    m_coord = move(other.m_coord);
    return *this;
}
#endif


