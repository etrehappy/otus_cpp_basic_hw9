#pragma once
#include <iostream>

template<typename T>
class u_vector 
{
public:
    u_vector();
    u_vector(const u_vector<T>& rhs);
    u_vector& operator=(const u_vector<T>& rhs) = delete; //нигде не используется
    u_vector(u_vector<T> &&rhs) noexcept; 
    u_vector& operator=(u_vector<T>&& rhs) noexcept; 
    virtual ~u_vector();

    void push_back(const T& value);
    size_t size() const;
    void erase(const size_t element_pos);
    void insert(const size_t pos, const T& value);

    struct iterator;
    iterator begin();
    iterator back();
    iterator end();

    T& operator[](int i) const;
    template<typename T>
    friend std::ostream& operator<<(std::ostream& out, u_vector<T>& rhs);

private:
    void move_from(u_vector&& rhs) noexcept;

    void double_capacity(); //доп. задание 1
    void new_container(const size_t old_size);
    void fill_new_arr(const size_t new_pos, const size_t new_size, const T& value, const T* ptr_old);
    void permutation(const size_t new_pos, const size_t new_size);
        
    size_t m_arr_size;
    T *m_arr;
    size_t m_capacity;
};

template<typename T>
struct u_vector<T>::iterator
{
public:
    iterator(T* p);

    T& operator*() const;
    iterator& operator++();   
    bool operator!=(iterator& rhs);

    T& get() const;

private:
    T* m_iterator_ptr;
};


#include "u_vector.ipp"

