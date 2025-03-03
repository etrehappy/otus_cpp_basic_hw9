#include "u_forward_list.h"

//u_forward_list's конструкторы и деструктор

u_forward_list::u_forward_list()
    : m_arr_size(0), m_start_node(nullptr), m_last_node(nullptr)
{
    std::cout << "\n 1. Cоздан объект класса u_forward_list для хранения элементов типа int в контейнере типа «Однонаправленный список».";
}

u_forward_list::u_forward_list(u_forward_list&& rhs) noexcept //доп. задание 3
{
    move_from(std::move(rhs));

    std::cout << "\n 1. Вызван конструктор перемещения u_forward_list(u_forward_list&& rhs)";
}

u_forward_list& u_forward_list::operator=(u_forward_list&& rhs) noexcept //доп. задание 3
{
    if (&rhs == this)
        return *this;

    move_from(std::move(rhs));

    std::cout << "\n 2. Вызван оператор присваивания перемещением operator=(u_forward_list&& rhs)";
    return *this;
}

u_forward_list::~u_forward_list()
{
    Node* it = m_start_node;
    Node* it_del = nullptr;

    while (it != nullptr)
    {
        it_del = it;
        it = it->next;

        delete it_del;
        it_del = nullptr;
    }
}


//Node's конструктор
u_forward_list::Node::Node()
    : m_data(0), next(nullptr)
{  
}

u_forward_list::Node::Node(int data)
    : m_data(data), next(nullptr)
{
}


//u_forward_list's public и friend функции
void u_forward_list::push_back(const int value)
{
    ++m_arr_size;

    Node* new_node = new Node{ value };   

    if (m_arr_size == 1)   
        m_start_node = new_node;
    else      
        m_last_node->next = new_node;

    m_last_node = new_node;
}

size_t u_forward_list::size() const
{
    return m_arr_size;
}

//@param pos > 0
void u_forward_list::erase(const size_t pos)
{   
    //check pos
    if (pos < 1 || pos > m_arr_size)
        throw Errors::ErrorPosition;    

    Node* erase_node = m_start_node;
    Node* node_prev = nullptr;

    if (erase_node == nullptr)
        throw Errors::EmptyContainer;

    //Search container[pos]       
    for (size_t count_pos = 1; count_pos != pos; ++count_pos)
    {
        node_prev = erase_node;
        erase_node = erase_node->next;        
    }

    //Change memory address
    if (pos == 1)    
        m_start_node = m_start_node->next;    
    else if (erase_node == nullptr) //check last position
        node_prev->next = nullptr;
    else
        node_prev->next = erase_node->next;  
        
    //Delete the old Node
    delete erase_node;
    erase_node = nullptr;

    --m_arr_size;
}

//@param pos > 0
void u_forward_list::insert(const size_t pos, const int value)
{
    //проверяем позицию
    if (pos < 1 || pos > m_arr_size)
        throw Errors::ErrorPosition;

    Node* old_node = m_start_node;
    Node* new_node = new Node{ value };       

    if (pos == 1) //вставка на первое место
    {
        m_start_node = new_node;
        new_node->next = old_node;
        goto size_count;
    }

    //Поиск места для вставки   
    Node* old_node_prev = nullptr;
    old_node = m_start_node->next;
    for (size_t count_pos = 2; count_pos != pos; ++count_pos) //старт со второго нода
    {
        old_node_prev = old_node;
        old_node = old_node->next;
    }
    
    //Вставка 
    if (old_node == nullptr)            //на последнее место
        old_node_prev->next = new_node;
    else                            //между первым и последним
    { 
        old_node_prev->next = new_node; //указываем next, проверил ошибку C6011 (old_node_prev != nullptr)
        new_node->next = old_node;
    }    

    size_count:
    ++m_arr_size;
}

typename u_forward_list::iterator u_forward_list::begin()
{
    return iterator(m_start_node);
}

typename u_forward_list::iterator u_forward_list::back()
{
    return iterator(m_last_node);
}

typename u_forward_list::iterator u_forward_list::end()
{    
    return iterator(m_last_node->next);
}

std::ostream& operator<<(std::ostream& out, u_forward_list& rhs)
{
    if (rhs.size() == 0)
        return out;

    auto last = rhs.back();

    for (auto it = rhs.begin(); it != last; ++it) //доп. задание 4  
        out << *it << ", ";

    out << *last;

    return out;
}


//u_forward_list's private функции
void u_forward_list::move_from(u_forward_list&& rhs) noexcept {
    m_start_node = rhs.m_start_node;
    m_last_node = rhs.m_last_node;
    m_arr_size = rhs.m_arr_size;

    rhs.m_start_node = rhs.m_last_node = nullptr;
    rhs.m_arr_size = 0;
}

//===========================================================================================================
//доп. задание 4   
//iterator's конструкторы и public методы

u_forward_list::iterator::iterator(Node* p)
    : m_iterator_ptr(p)
{}

int& u_forward_list::iterator::operator*()
{
    return m_iterator_ptr->m_data;
}

typename u_forward_list::iterator& u_forward_list::iterator::operator++()
{
    m_iterator_ptr = m_iterator_ptr->next;
    return *this;
}

bool u_forward_list::iterator::operator!=(typename u_forward_list::iterator& rhs)
{
    return m_iterator_ptr != rhs.m_iterator_ptr;
}

int& u_forward_list::iterator::get()
{
    return m_iterator_ptr->m_data;
}