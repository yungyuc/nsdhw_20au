#include <cstdint>
#include <algorithm>
#include <utility>
#include <cmath>
#include <iostream>
#include<vector>

using namespace std;

class Line
{
public:
    // Basic constructors.
    Line() = default; // default constructor.
    Line(const Line & ); // copy constructor.
    Line(Line       &&); // move constructor.
    Line & operator=(const Line & ); // copy assignment operator.
    Line & operator=(Line       &&); // move assignment operator.
    // Custom constructor.
    Line(size_t size) : m_size(size) {m_coord.resize(size*2);}
    // Desctructor.
    ~Line() { if (!m_coord.empty()) { m_coord.clear(); } }
    // Accessors.
    size_t size() const { return m_size; }
    float getx(size_t it) const { check_range(it); return m_coord.at(it*2  ); }
    void setx(size_t it,float value) { check_range(it); m_coord.at(it*2) = value; }
    float gety(size_t it) const { check_range(it); return m_coord.at(it*2+1); }
    void sety(size_t it,float value) { check_range(it); m_coord.at(it*2+1) = value; }
private:
    // Private helper.
    void check_range(size_t it) const
    {
        try { m_coord.at(it*2); } // Bound check.
        catch (std::out_of_range e)
        { cout << "out of range exception: " << e.what() << std::endl; } }
    // Member data.
    size_t m_size = 0; // number of points.
    vector<float> m_coord; // memory buffer for the points.
}; /* end class Line */

/* Define the copy constructor */
Line::Line(const Line & other)
{
    if (!other.m_coord.empty())
    {
        if (!m_coord.empty()) { m_coord.clear(); } // free unused buffer.
        m_coord.resize(other.m_size*2);
        m_size = other.m_size;
        m_coord.assign(other.m_coord.begin(),other.m_coord.end());
    }
    else // the other object is empty.
    {
        if (!m_coord.empty())
        {
            m_coord.clear(); // free unused buffer.
            m_size = 0;
        }
    }
}

/* Define the move constructor */
Line::Line(Line && other)
{
    swap(other.m_size, m_size);
    swap(other.m_coord, m_coord);
}

/* Define the copy assignment operator */
Line & Line::operator=(const Line & other)
{
    if (this == &other) { return *this; } // don't move to self.
    // This part is the same as what's in the copy constructor.
    if (!other.m_coord.empty())
    {
        if (!m_coord.empty()) { m_coord.clear(); }
        m_coord.resize(other.m_size*2);;
        m_size = other.m_size;
        m_coord.assign(other.m_coord.begin(),other.m_coord.end());
    }
    else
    {
        if (!m_coord.empty())
        {
            m_coord.clear();
            m_size = 0;
        }
    }
    return *this;
}

/* Define the move assignment operator */
Line & Line::operator=(Line && other)
{
    if (this == &other) { return *this; } // don't move to self.
    swap(other.m_size, m_size);
    swap(other.m_coord, m_coord);
    return *this;
}

int main(int, char **)
{
    Line line(3);
    line.setx(0,0); line.sety(0,1);
    line.setx(1,1); line.sety(1,3);
    line.setx(2,2); line.sety(2,5);

    Line line2(line);
    line2.setx(0,9);

    cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        cout << "point " << it << ":"
                  << " x = " << line.getx(it)
                  << " y = " << line.gety(it) << std::endl;
    }

    cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        cout << "point " << it << ":"
                  << " x = " << line2.getx(it)
                  << " y = " << line2.gety(it) << std::endl;
    }

    return 0;
}
