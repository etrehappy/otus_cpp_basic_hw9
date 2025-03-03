#include "std_deviation.h"

Std_deviation::Std_deviation()
	: m_std(NULL), m_summ_all(NULL), m_mean(NULL), m_dispersion(NULL), m_count(NULL), m_vector{}
{}

void Std_deviation::update(double value)
{
	++m_count;
	m_summ_all = m_summ_all + value;
	m_mean = m_summ_all / m_count;
	m_vector.push_back(value);

	double squared_deviation(0.);

	for (int i = 0; i < m_vector.size(); i++)
		squared_deviation += pow(m_vector.at(i) - m_mean,2);
	
	m_dispersion = squared_deviation / m_count;
	m_std = sqrt(m_dispersion);
}

double Std_deviation::get_result() const
{	
		return m_std;
}

char* Std_deviation::get_statistic_name() const
{
	return "std";
}