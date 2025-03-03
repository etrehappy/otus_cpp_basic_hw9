#include "most_common_words.h"
    
std::mutex MostCommonWords::mutex_vector_{};
std::unique_lock<std::mutex> MostCommonWords::guard_mutex_{ mutex_vector_, std::defer_lock };

MostCommonWords::MostCommonWords(const size_t max_words)
    :volume_vec_{}, volume_map_{}, max_words_{max_words}
{    
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  Method:   MostCommonWords::AddWords
  Summary:  Prepares containers (a map and a vector) for print.
  Modifies: [volume_map_, volume_vec_].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void MostCommonWords::AddWords(const std::vector<std::string>& arguments)
{
    std::vector<std::thread> threads{};
    
    for (auto& it : arguments)    
        threads.emplace_back(&MostCommonWords::FillVolumeMap, this, std::ref(it));
    
    for (auto& thread : threads)    
        thread.join();
    
    FillVolumeVec();
    SortVector();
}

//Whithout threads
void MostCommonWords::PrintTop(std::ostream& stream)
{
    std::for_each(std::begin(volume_vec_), std::begin(volume_vec_) + max_words_,
        [&stream](const std::pair<std::string, std::size_t>& pair)
        {
            stream << std::setw(4) << pair.second << " " << pair.first << '\n';
        });
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  Method:   MostCommonWords::FillVolumeMap
  Summary:  Fills page's map-container and move result into this->map-container.
            Every page opens in a new thread.
  Modifies: [Page::words_page_, MostCommonWords::volume_map_].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void MostCommonWords::FillVolumeMap(const std::string& filepath)
{
    std::ifstream input_file{ filepath };
    if (!input_file.is_open())
    {
        std::cerr << "Failed to open file " << filepath << '\n';
        std::exit(EXIT_FAILURE);
    }

    Page page{};
    page.AddWords(input_file);
    MergePage(page);
}

//Whithout threads
void MostCommonWords::FillVolumeVec()
{            
    size_t vector_size = volume_vec_.size() + volume_map_.size();
    volume_vec_.reserve(vector_size);

    for (auto& pair : volume_map_)
    {
        volume_vec_.emplace_back(std::move(pair));
    } 
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  Method:   MostCommonWords::MergePage
  Summary:  Fills page's map-container and move result into this->map-container.
            Every page opens in a new thread.
  Modifies: [Page::words_page_, MostCommonWords::volume_map_].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void MostCommonWords::MergePage(Page& p)
{
    guard_mutex_.lock();
    for (auto& pair : p.GetPageNoConst())
    {
        auto it = MostCommonWords::volume_map_.find(pair.first);
        if (it != MostCommonWords::volume_map_.end())
            it->second += pair.second;
        else
            MostCommonWords::volume_map_.insert(std::move(pair));
    }
    guard_mutex_.unlock();
    p.Clear();
}

//Whithout threads
void MostCommonWords::SortVector()
{
    std::partial_sort(std::begin(volume_vec_),
        std::begin(volume_vec_) + max_words_, std::end(volume_vec_),
        [](auto lhs, auto& rhs) {return (lhs.second) > (rhs.second); });
}




MostCommonWords::Page::Page()
    : words_page_{}
{
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
    Method:   Page::AddWords
    Summary:  Adds words in the map-container and counts them.
                Each word is already in lower case.
    Modifies: words_page_
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void MostCommonWords::Page::AddWords(std::istream& input_stream)
{
    std::for_each(std::istream_iterator<std::string>(input_stream),
        std::istream_iterator<std::string>(),
        [this](const std::string& s) { ++words_page_[ToLower(s)]; });
}

std::string MostCommonWords::Page::ToLower(const std::string& str)
{
    std::string lower_str;
    std::transform(std::cbegin(str), std::cend(str), std::back_inserter(lower_str),
        [](unsigned char ch) {return std::tolower(ch); });

    return lower_str;
};

volume_str_map& MostCommonWords::Page::GetPageNoConst()
{
    return words_page_;
}

void MostCommonWords::Page::Clear()
{
    words_page_.clear();
}

