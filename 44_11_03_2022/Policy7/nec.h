#pragma once
#include <iosfwd>

class Nec {
public:
	Nec(int = 0);
	~Nec();
	
	void set(int val)
	{
		m_val = val;
	}
	int get()const
	{
		return m_val;
	}
	friend std::ostream& operator<<(std::ostream&, const Nec&);
private:
	int m_val;
};