#include "u_vector.h"

//u_vector's конструкторы и деструктор

template<typename T>
u_vector<T>::u_vector()
    : m_arr_size(0), m_arr(nullptr), m_capacity(0)
{
}

template<typename T>
u_vector<T>::u_vector(const u_vector<T>& rhs)
    : m_arr_size(rhs.m_arr_size), m_arr(new T[rhs.m_arr_size]), m_capacity(rhs.m_capacity)
{
    for (size_t i = 0; i < m_arr_size; i++)
        m_arr[i] = rhs.m_arr[i];
}

template<typename T>
u_vector<T>::u_vector(u_vector<T>&& rhs) noexcept 
{
    move_from(std::move(rhs));
}

template<typename T>
u_vector<T>& u_vector<T>::operator=(u_vector<T>&& rhs) noexcept 
{
    if (&rhs == this) 
        return *this;

    move_from(std::move(rhs));
    
    return *this;
}

template<typename T>
u_vector<T>::~u_vector()
{
    delete[] m_arr;      
}



//u_vector's public и friend функции

template<typename T>
void u_vector<T>::push_back(const T& value)
{  
    const size_t old_size = m_arr_size;
    ++m_arr_size;

    if (m_capacity <= old_size)
    {
        double_capacity();          
        new_container(old_size);
        m_arr[old_size] = value;
    }
    else
        m_arr[old_size] = value;
}

template<typename T>
size_t u_vector<T>::size() const
{
    return m_arr_size;
}

//@param element_pos > 0
template<typename T>
void u_vector<T>::erase(const size_t element_pos)
{   
    //проверяем позицию
    if (element_pos < 1 || element_pos > m_arr_size)    
        return;

    //смещаем элементы
    for (size_t pre_pos = element_pos - 1; pre_pos < m_arr_size; pre_pos++)
    {
        m_arr[pre_pos] = m_arr[pre_pos + 1];
    }    
    --m_arr_size;
}

//@param pos > 0
template<typename T> 
void u_vector<T>::insert(const size_t pos, const T& value)
{
    if (pos < 1 || pos > m_arr_size)        
        return;

    const size_t new_pos = pos - 1;
    const size_t old_size = m_arr_size;
    const size_t new_size = ++m_arr_size;
    
    T* arr_old = m_arr;

    if (m_capacity <= old_size)
    {
        double_capacity();        
        m_arr = new T[new_size];
        fill_new_arr(new_pos, new_size, value, arr_old);

        delete[] arr_old;
    }
    else
    {
        permutation(new_pos, old_size);
        m_arr[new_pos] = value;
    }
}

template<typename T>
typename u_vector<T>::iterator u_vector<T>::begin()
{
    return iterator(m_arr);
}

template<typename T>
typename u_vector<T>::iterator u_vector<T>::back()
{
    return iterator(m_arr + m_arr_size - 1);
}

template<typename T>
typename u_vector<T>::iterator u_vector<T>::end()
{
    return iterator(m_arr + m_arr_size);
}


template<typename T>
T& u_vector<T>::operator[](int i) const
{
    if (i > (m_arr_size - 1))
        Errors::OutOfBounds;

    return m_arr[i];
}

template<typename T>
std::ostream& operator<<(std::ostream& out, u_vector<T>& rhs)
{
    if (rhs.m_arr == nullptr)
        return out;

    auto last = rhs.back();
    
    for (auto it = rhs.begin(); it != last; ++it)  //доп. задание 4   
        out << *it << ", ";

    out << *last;
    return out;
}



//u_vector's private методы

template<typename T>
void u_vector<T>::move_from(u_vector<T>&& rhs) noexcept {
    m_arr = rhs.m_arr;
    m_arr_size = rhs.m_arr_size;
    m_capacity = rhs.m_capacity;

    rhs.m_arr = nullptr;
    rhs.m_arr_size = 0;
    rhs.m_capacity = 0;
}

template<typename T>
void u_vector<T>::double_capacity()
{
    m_capacity = 2 * m_arr_size;
}

template<typename T>
void u_vector<T>::new_container(const size_t old_size) 
{
    auto new_array = new T[m_capacity]{ 0 };
    for (size_t i = 0; i < old_size; ++i) 
        new_array[i] = m_arr[i]; 
    
    delete[] m_arr;
    m_arr = new_array;
}

template<typename T>
void u_vector<T>::fill_new_arr(const size_t new_pos, const size_t new_size, const T& value, const T* arr_old)
{ 
    for (int i = 0; i < new_size; i++)
    {
        if (i == new_pos)
            m_arr[i] = value; //вставить new value в new_pos
        else if (i > new_pos)
            m_arr[i] = arr_old[i - 1]; //вставка от места (new value + 1) до end
        else
            m_arr[i] = arr_old[i];  //вставка от 0 до new value 
    }
}

template<typename T>
void u_vector<T>::permutation(const size_t new_pos, size_t old_end)
{
    //меняем элемнты местами от end до new value
    for (; old_end != new_pos; old_end--)          
        m_arr[old_end] = m_arr [old_end-1];
}


//===========================================================================================================
//iterator's конструкторы и public методы
template<typename T>
u_vector<T>::iterator::iterator(T* p)
    : m_iterator_ptr(p)
{}

template<typename T>
T& u_vector<T>::iterator::operator*() const
{
    return *m_iterator_ptr;
}

template<typename T>
typename u_vector<T>::iterator& u_vector<T>::iterator::operator++()
{
    ++m_iterator_ptr;
    return *this;
}

template<typename T>
bool u_vector<T>::iterator::operator!=(typename u_vector<T>::iterator& rhs)
{
    return  m_iterator_ptr != rhs.m_iterator_ptr;
}

template<typename T>
T& u_vector<T>::iterator::get() const
{
    return *m_iterator_ptr;
}