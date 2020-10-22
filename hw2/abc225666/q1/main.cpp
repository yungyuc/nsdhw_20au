#include <bits/stdc++.h>


class Point
{
public:
    friend class Line;
private:
    float x;
    float y;
};

class Line
{
public:
    Line();
    Line(Line const & );
    Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    Line(size_t size);
    ~Line() {};
    size_t size() {
        return points.size();
    }
    float x(size_t it) const;
    float & x(size_t it);
    float y(size_t it) const;
    float & y(size_t it);

private:
    std::vector<Point> points;
    // Member data.
}; /* end class Line */

Line::Line(Line const & other) {
    points = other.points;
}

Line::Line(Line&& other) {
    points = std::move(other.points);
    std::cout<<"move\n";
}

Line & Line::operator=(Line const &other) {
    if(this==&other) {
        return *this;
    }
    points = other.points;
    return *this;
}

Line & Line::operator=(Line && other) {
    if(this==&other) {
        return *this;
    }
    points = std::move(other.points);
    return *this;
}

Line::Line(size_t size) {
    points.resize(size);
}

float Line::x(size_t it) const {
    return points.at(it).x;
}
float & Line::x(size_t it) {
    return points.at(it).x;
}
float Line::y(size_t it) const {
    return points.at(it).y;
}
float & Line::y(size_t it) {
    return points.at(it).y;
}


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
