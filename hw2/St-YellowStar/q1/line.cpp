#include "line.hpp"

Line::Line(size_t size){
    points.resize(size);
    for(auto it=points.begin();it!=points.end();it++){
        *it = new Point();
    }
}

Line::Line(const Line& other): Line(other.size()){
    for(size_t i = 0; i < other.size(); i++){
        *(points.at(i)) = *(other.points.at(i));
    }
}

Line::Line(Line&& other){
    *this = other;
}

Line& Line::operator=(const Line& other){
    *this = Line(other);
    return *this;
}

Line& Line::operator=(Line&& other){
    std::swap(points, other.points);
    return *this;
}

size_t Line::size() const{
    return points.size();
}

float& Line::x(size_t it) const{
    return points.at(it)->x;
}

float& Line::x(size_t it){
    return points.at(it)->x;
}

float& Line::y(size_t it) const{
    return points.at(it)->y;
}

float& Line::y(size_t it){
    return points.at(it)->y;
}
