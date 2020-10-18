#pragma once

#include <vector>
#include <cstdint>
#include <iostream>



struct Point{
    float x = 0;
    float y = 0;
};

class Line{

    public:

        // Basic constructors.
        Line() = default; // default constructor.
        Line(Line const & ); // copy constructor.
        Line(Line       &&); // move constructor.
        Line & operator=(Line const & ); // copy assignment operator.
        Line & operator=(Line       &&); // move assignment operator.

        // Custom constructor.
        Line(size_t size);

        // Desctructor.
        ~Line() {}

        // Accessors.
        size_t size() const { return points.size(); }
        float & x(size_t it) { return points.at(it).x; }
        float & y(size_t it) { return points.at(it).y; }

    private:

        std::vector<Point> points;
};

// Define the custom constructor
Line::Line(size_t size){
    points.resize(size);
}

// Define the copy constructor
Line::Line(Line const & other){
    points = other.points;
}

// Define the move constructor
Line::Line(Line && other)
{
    std::swap(other.points, points);
}

// Define the copy assignment operator 
Line & Line::operator=(Line const & other)
{
    if (this == &other) { return *this; } // don't move to self.
    // This part is the same as what's in the copy constructor.
    points = other.points;
    return *this;
}

// Define the move assignment operator 
Line & Line::operator=(Line && other)
{
    if (this == &other) { return *this; } // don't move to self.
    std::swap(other.points, points);
    return *this;
}

