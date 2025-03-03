#include "max.h"

Max::Max()
	: m_max(NULL), m_empty(true)
{}

void Max::update(double value)
{
	if (m_empty)
	{
		m_max = value;
		m_empty = false;
	}
	else if (value > m_max)
		m_max = value;	
}

double Max::get_result() const
{
	return m_max;
}

char* Max::get_statistic_name() const
{
	return "max";
}
