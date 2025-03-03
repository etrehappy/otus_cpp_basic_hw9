#include "main.h"

int MyTopK(int argc, char* argv[])
{
    constexpr size_t max_words_in_ranking = 10;
    MostCommonWords dictionary{ max_words_in_ranking };

    std::vector<std::string> arguments{};
    for (int i = 1; i < argc; ++i)
       arguments.push_back(argv[i]);

    dictionary.AddWords(arguments);    
    dictionary.PrintTop(std::cout);

    return 0;
}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: GetTimeAndPrintTop
  Summary:  Получает минимальное время за call_foo_counter попыток и запускает
            основной код. Это примитивный вариант, чтобы автоматически сравнить
            результат оригинальной программы из домашнего задания с передаланной
            мною. Другие варианты потребуют либо новые функции, либо изменение 
            кода в оригинальной программе, либо Google test, либо ручной тест. 
            Поэтому прошу это не оценивать :) 
-----------------------------------------------------------------F-F*/
long long GetTimeAndPrintTop(int argc, char* argv[], int(*func)(int, char**))
{
    constexpr size_t call_foo_counter = 1;

    std::vector<std::chrono::microseconds::rep> time_results{};
    for (size_t j = 0; j < call_foo_counter; ++j)
    {
        const auto start = std::chrono::high_resolution_clock::now();

        func(argc, argv);

        const auto end = std::chrono::high_resolution_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        time_results.push_back(elapsed_ms.count());        
    }

    return *std::min_element(time_results.cbegin(), time_results.cend()); 
}


int main(int argc, char *argv[]) 
{
    
    std::locale::global(std::locale("en_US.utf8"));

    if (argc < 2) {
        std::cerr << "Usage: topk_words [FILES...]\n";
        return EXIT_FAILURE;
    } 
    
    std::cout << "\t=======\tSTART ORIGINAL PROGRAM\t=====\n";
    const auto original_time = GetTimeAndPrintTop(argc, argv, topk_words_example_hw);
    std::cout << "Original elapsed time is " << original_time << " us\n\n";
        
    std::cout << "\t=======\tSTART MY VERSION\t=====\n";
    const auto my_time = GetTimeAndPrintTop(argc, argv, MyTopK);
    std::cout << "My elapsed time is " << my_time << " us\n";
    
    return 0;
}


//                  Минимальное время за 5 проходов
// Программа    |      1 поток      |   Многопоток
//  Оригинал    |   2'139'082 us    |       -
//  Моя         |   1'286'366 us    |       790509 us
      