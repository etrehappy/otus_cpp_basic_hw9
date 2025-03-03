#pragma once
#include "statistics.h"

class Mean : public IStatistics
{
public:
    Mean();

    void    update(double value) override;
    double  get_result() const override;
    char* get_statistic_name() const override;

private:
    double m_mean;
    double m_summ_all;
    int m_count;
};