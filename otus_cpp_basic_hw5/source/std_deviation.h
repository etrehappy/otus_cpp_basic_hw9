#pragma once
#include "statistics.h"

class Std_deviation : public IStatistics
{
public:
    Std_deviation();

    void    update(double value) override;
    double  get_result() const override;
    char* get_statistic_name() const override;

private:
    double m_std;
    double m_summ_all;
    double m_mean;
    double m_dispersion;
    int m_count;

    std::vector<double> m_vector;
};
