#include "line.h"

Line::Line(Line const &other) {
  m_size = other.m_size;
  m_coord = other.m_coord;
}

Line::Line(Line &&other) {
  std::swap(other.m_size, m_size);
  other.m_coord.swap(m_coord);
}

Line &Line::operator=(Line const &other) {
  if (this != &other) {
    m_size = other.m_size;
    m_coord = other.m_coord;
  }
  return *this;
}

Line &Line::operator=(Line &&other) {
  if (this != &other) {
    std::swap(other.m_size, m_size);
    other.m_coord.swap(m_coord);
  }
  return *this;
}

Line::Line(size_t size) : m_size(size), m_coord(2 * size) {}

size_t Line::size() const { return m_size; }

const float &Line::x(size_t it) const { return m_coord.at(2 * it); }

float &Line::x(size_t it) { return m_coord.at(2 * it); }

const float &Line::y(size_t it) const { return m_coord.at(2 * it + 1); }

float &Line::y(size_t it) { return m_coord.at(2 * it + 1); }
