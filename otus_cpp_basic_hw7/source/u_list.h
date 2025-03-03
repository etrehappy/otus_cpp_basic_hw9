#pragma once
#include <iostream>

template<typename T>
class u_list
{
public:
    u_list();
    u_list(const u_list&);
    u_list& operator=(const u_list&) = delete; //нигде не используется
    u_list(u_list&& rhs) noexcept; 
    u_list& operator=(u_list&& rhs) noexcept; 
    virtual ~u_list();

    void push_back(const T& value);
    size_t size() const;
    void erase(const size_t element_pos);
    void insert(const size_t pos, const int value);
    
    struct iterator;
    iterator begin();
    iterator cbegin() const;
    iterator back();
    iterator cback() const;
    iterator end();
    template<typename T>
    friend std::ostream& operator<<(std::ostream& out, u_list& rhs);    

private:
    void move_from(u_list&& rhs) noexcept;

    struct Node;

    size_t m_arr_size;
    Node* m_start_node;    
    Node* m_last_node;
};

template<typename T>
struct u_list<T>::iterator
{
public:
    iterator(Node* p);

    int& operator*();
    iterator& operator++();
    bool operator!=(/*const*/ iterator& rhs);
    Node* operator->();

    int& get();

private:    
    Node* m_iterator_ptr;
};

template<typename T>
struct u_list<T>::Node
{
    Node* next;
    Node* prev;
    int m_data;

    Node();
    Node(int data);
    ~Node() = default;
};

#include "u_list.ipp"