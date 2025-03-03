#pragma once
#include <iostream>
#include "my_exception.h"

//Только для int
class u_forward_list
{
public:
    u_forward_list();
    u_forward_list(const u_forward_list& rhs) = delete;
    u_forward_list& operator=(const u_forward_list& rhs) = delete;
    u_forward_list(u_forward_list&& rhs) noexcept;
    u_forward_list& operator=(u_forward_list&& rhs) noexcept;
    virtual ~u_forward_list();

    void push_back(const int value);
    size_t size() const;
    void erase(const size_t pos);
    void insert(const size_t pos, const int value);

    struct iterator;
    iterator begin();
    iterator back();
    iterator end();

    friend std::ostream& operator<<(std::ostream& out, u_forward_list& rhs);    

private:  
    void move_from(u_forward_list&& rhs) noexcept;

    struct Node;

    size_t m_arr_size;
    Node* m_start_node;
    Node* m_last_node; //для push_back и end
};

struct u_forward_list::iterator
{
public:
    iterator(Node* p);

    int& operator*();
    iterator& operator++();
    bool operator!=(iterator& rhs);

    int& get();

private:
    Node* m_iterator_ptr;
};

struct u_forward_list::Node
{
    Node* next;
    int m_data;

    Node();
    Node(int data);
    ~Node() = default;
};

