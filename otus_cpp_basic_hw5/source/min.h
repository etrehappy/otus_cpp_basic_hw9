#pragma once
#include "statistics.h"

class Min : public IStatistics
{
public:
    Min();

    void    update(double value) override;
    double  get_result() const override;
    char* get_statistic_name() const override;

private:
    double m_min;
    bool m_empty;
};