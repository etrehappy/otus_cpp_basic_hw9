#include "statistics.h"
#include "min.h"
#include "max.h"
#include "mean.h"
#include "std_deviation.h"
#include "pct90.h"
#include "pct95.h"

enum class Statistic_Types
{
    Min,
    Max,
    Mean,
    Std_deviation,
    Percentile_90,
    Percentile_95,

    max_count,
};

void array_initialization(IStatistics* statistics[])
{
    statistics[(int)Statistic_Types::Min] = new Min{};
    statistics[(int)Statistic_Types::Max] = new Max{};
    statistics[(int)Statistic_Types::Mean] = new Mean{};
    statistics[(int)Statistic_Types::Std_deviation] = new Std_deviation{};
    statistics[(int)Statistic_Types::Percentile_90] = new Percentile_90{};
    statistics[(int)Statistic_Types::Percentile_95] = new Percentile_95{};
}

void get_numbers(IStatistics* statistics[])
{
    double value;

    std::cout << "Enter any sequence of numbers: ";
    while (std::cin >> value)
    {
        for (auto type = (int)Statistic_Types::Min; type < (int)Statistic_Types::max_count; type++)
            statistics[type]->update(value);

        if (std::cin.peek() == '\n')
        {
            #if defined(WIN32)
                        std::cout << "\nPush Ctrl+Z -> Enter for result: ";
            #else
                        std::cout << "\nPush Ctrl+D for result: ";
            #endif
        }
    }

    // Handle invalid input data
    if (!std::cin.eof() && !std::cin.good()) {
        std::cerr << "Invalid input data\n";
        exit(-1);
    }
}

void print_result(IStatistics* statistics[])
{
    for (auto type = (int)Statistic_Types::Min; type < (int)Statistic_Types::max_count; type++)
    {
        std::cout << "\nResult of " << statistics[type]->get_statistic_name() << " is \t" << statistics[type]->get_result();
    }
}

void delete_ptrs(IStatistics* statistics[])
{
    for (auto type = (int)Statistic_Types::Min; type < (int)Statistic_Types::max_count; type++)
    {
        delete statistics[type];
    }
}

int main()
{
    IStatistics* statistics[(int)Statistic_Types::max_count];

    array_initialization(statistics);
    
    get_numbers(statistics);
    
    print_result(statistics);

    std::cout << std::endl << std::endl;

    delete_ptrs(statistics);

    return 0;
}