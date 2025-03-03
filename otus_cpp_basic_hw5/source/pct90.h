#pragma once
#include "statistics.h"

class Percentile_90 : public IStatistics
{
public:
    Percentile_90();

    void    update(double value) override;
    double  get_result() const override;
    char* get_statistic_name() const override;

private:
    double m_pct90;
    std::vector<double> m_vector;
};