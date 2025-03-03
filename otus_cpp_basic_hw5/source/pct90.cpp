#include "pct90.h"

Percentile_90::Percentile_90()
	: m_pct90(NULL), m_vector{}
{}

void Percentile_90::update(double value) 
{
	m_vector.push_back(value);
	
	int i = ceil(90. * m_vector.size() / 100.); // n = (P/100) * N, 0 < P <= 100

	if (i < 2)
		m_pct90 = m_vector[0];
	else
		m_pct90 = m_vector[i - 1];
}

double Percentile_90::get_result() const
{
	return m_pct90;
}

char* Percentile_90::get_statistic_name() const
{
	return "pct90";
}