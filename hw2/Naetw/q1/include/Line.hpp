#ifndef __LINE
#define __LINE

#include <vector>

struct Point {
    float x;
    float y;
};

class Line {
  private:
    std::vector<Point> m_coordinate;

    typedef std::vector<Point>::size_type coord_size_t;

  public:
    ~Line() = default;

    Line(coord_size_t size);

    Line(const Line &other);
    Line &operator=(const Line &other);

	coord_size_t size() const;

    const float &x(coord_size_t idx) const;
    float &x(coord_size_t idx);

    const float &y(coord_size_t idx) const;
    float &y(coord_size_t idx);
};

#endif
