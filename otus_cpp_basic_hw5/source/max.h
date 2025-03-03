#pragma once
#include "statistics.h"

class Max : public IStatistics
{
public:
    Max();

    void    update(double value) override;
    double  get_result() const override;
    char* get_statistic_name() const override;

private:
    double m_max;
    bool m_empty;

};