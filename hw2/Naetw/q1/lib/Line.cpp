#include <Line.hpp>

Line::Line(coord_size_t size) : m_coordinate(size) {}


Line::Line(const Line &other) : m_coordinate(other.m_coordinate) {}

Line &Line::operator=(const Line &other) {
    if (this == &other) {
        return *this;
    }

    m_coordinate = other.m_coordinate;
    return *this;
}

Line::coord_size_t Line::size() const { return m_coordinate.size(); }

const float &Line::x(coord_size_t idx) const {
    return m_coordinate.at(idx).x;
}

float &Line::x(coord_size_t idx) {
	return const_cast<float &>(static_cast<const Line&>(*this).x(idx));
}

const float &Line::y(coord_size_t idx) const {
    return m_coordinate.at(idx).y;
}

float &Line::y(coord_size_t idx) {
	return const_cast<float &>(static_cast<const Line&>(*this).y(idx));
}
