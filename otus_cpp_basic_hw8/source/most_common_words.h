#pragma once

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>


using volume_str_map = std::map<std::string, std::size_t>;

class MostCommonWords
{
public:
    MostCommonWords(const size_t max_words_);

    MostCommonWords(const MostCommonWords&) = delete;           //doesn't use in the project
    MostCommonWords& operator=(const MostCommonWords&) = delete;//doesn't use in the project
    MostCommonWords(MostCommonWords&&) = delete;                //doesn't use in the project
    MostCommonWords& operator=(MostCommonWords&&) = delete;     //doesn't use in the project
    ~MostCommonWords() = default;    
    
    void AddWords(const std::vector<std::string>& arguments);
    void PrintTop(std::ostream& stream);    

protected:
    static std::mutex mutex_vector_;
    static std::unique_lock<std::mutex> guard_mutex_;    

private:
    class Page;
    const size_t max_words_;
    std::vector<std::pair<std::string, std::size_t>> volume_vec_;
    volume_str_map volume_map_;

    void FillVolumeMap(const std::string& filepath);
    void FillVolumeVec();
    void MergePage(Page&);
    void SortVector();
};

class MostCommonWords::Page
{
public:
    Page();
    ~Page() = default;

    void AddWords(std::istream& input_stream);
    std::string ToLower(const std::string& str);
    volume_str_map& GetPageNoConst();
    void Clear();

private:
    volume_str_map words_page_;        
};