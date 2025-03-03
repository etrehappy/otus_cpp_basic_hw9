#pragma once
#include "statistics.h"

class Percentile_95 : public IStatistics
{
public:
    Percentile_95();

    void    update(double value) override;
    double  get_result() const override;
    char* get_statistic_name() const override;

private:
    double m_pct95;
    std::vector<double> m_vector;
};