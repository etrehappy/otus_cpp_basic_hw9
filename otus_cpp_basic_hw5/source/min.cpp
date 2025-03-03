#include "min.h"

Min::Min() 
    : m_min(NULL), m_empty(true)
{}

void Min::update(double value)
{
	if (m_empty)
	{
		m_min = value;
		m_empty = false;
	}
	else if (value < m_min)	
		m_min = value;
}

double Min::get_result() const
{
	return m_min;
}

char* Min::get_statistic_name() const
{
	return "min";
}

