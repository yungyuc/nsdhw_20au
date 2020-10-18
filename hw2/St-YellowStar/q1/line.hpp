#pragma once

#include <vector>
#include <iostream>

class Line{
    public:
        Line() = default;
        Line(size_t);
        Line(const Line&);
        Line(Line&& other);
        Line& operator=(const Line&);
        Line& operator=(Line&&);
        ~Line() = default;

        size_t size() const;
        float& x(size_t) const;
        float& x(size_t);
        float& y(size_t) const;
        float& y(size_t);

    private:
        struct Point{
            float x;
            float y;

            Point():x(0), y(0){}
        };
        std::vector<Point*> points;
};
