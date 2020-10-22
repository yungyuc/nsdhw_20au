#include <iostream>
#include <cmath>
#include <vector> 

using namespace std;

class Line
{
public:
    Line();

    Line(Line const & l) {
	_x = l._x;
	_y = l._y;
    }

    Line(Line &&);

    Line & operator=(Line const &);
    Line & operator=(Line &&);

    Line(size_t size) {
	_size = size;
	_x.resize(size);
	_y.resize(size);
    }

    ~Line() {
	_x.clear();
	_y.clear();
    }

    size_t size() const{
	return _size;
    }

    const float & x(size_t it) const{
	return _x.at(it);
    }

    float & x(size_t it){
	return _x.at(it);
    }

    const float & y(size_t it) const {
	return _y.at(it);
    }

    float & y(size_t it){
	return _y.at(it);
    }

private:
    std::vector<float> _x;
    std::vector<float> _y;
    size_t _size;
};

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
