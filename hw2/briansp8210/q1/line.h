#ifndef LINE_H_
#define LINE_H_

#include <vector>

class Line {
public:
  Line();
  Line(Line const &);
  Line(Line &&);
  Line &operator=(Line const &);
  Line &operator=(Line &&);
  Line(size_t size);
  ~Line() {};
  size_t size() const;
  const float &x(size_t it) const;
  float &x(size_t it);
  const float &y(size_t it) const;
  float &y(size_t it);

private:
  size_t m_size;
  std::vector<float> m_coord;
};

#endif // LINE_H_
