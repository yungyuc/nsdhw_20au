#ifndef    LINE_HPP
#define    LINE_HPP
#include  <vector>

class Line
{
	public:
		Line();
		Line(Line const & other) : m_size(other.m_size), m_coord(other.m_coord) {}
		Line(Line      && other) : m_size(other.m_size), m_coord(move(other.m_coord)) {}
		Line & operator=(Line const & );
		Line & operator=(Line       &&);
		Line(size_t size) { m_size = size; for (size_t t=0; t<m_size*2; t++) { m_coord.push_back(0); }}
		~Line() {}
		size_t size() const { return m_size; }
		int   x(size_t it) const { check_range(it); return m_coord[it*2  ]; }
		int & x(size_t it)       { check_range(it); return m_coord[it*2  ]; }
		int   y(size_t it) const { check_range(it); return m_coord[it*2+1]; }
		int & y(size_t it)	   { check_range(it); return m_coord[it*2+1]; }
	private:
		void check_range(size_t it) const
		{ if (it >= m_size) { throw std::out_of_range("Line index out of range");}}
		size_t m_size = 0;
		std::vector<int> m_coord;
};


Line & Line::operator=(Line const & rhs)
{
	if (this != &rhs)
	{
		m_size = rhs.m_size;
		m_coord = rhs.m_coord;
	}
	return *this;
}


Line & Line:: operator=(Line && rhs)
{
	if ( this != &rhs)
	{
		m_size = rhs.m_size;
		m_coord = move(rhs.m_coord);
	}
	return *this;
}

#endif
