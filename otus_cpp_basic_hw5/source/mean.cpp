#include "mean.h"

Mean::Mean()
	: m_mean(NULL), m_summ_all(NULL), m_count(NULL)
{}

void Mean::update(double value)
{
		++m_count;
		m_summ_all = m_summ_all + value;
		m_mean = m_summ_all / m_count;
}

double Mean::get_result() const
{
		return m_mean;	
}

char* Mean::get_statistic_name() const
{
	return "mean";
}
