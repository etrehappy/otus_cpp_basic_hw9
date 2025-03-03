#include "pct95.h"

Percentile_95::Percentile_95()
	: m_pct95(NULL), m_vector{}
{}

void Percentile_95::update(double value) 
{
	m_vector.push_back(value);

	int i = ceil(95. * m_vector.size() / 100.); // n = (P/100) * N, 0 < P <= 100

	if (i < 2 )
		m_pct95 = m_vector[0];
	else
		m_pct95 = m_vector[i - 1];
}

double Percentile_95::get_result() const
{
	return m_pct95;
}

char* Percentile_95::get_statistic_name() const
{
	return "pct95";
}