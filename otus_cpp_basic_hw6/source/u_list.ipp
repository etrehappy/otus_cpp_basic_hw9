#include "u_list.h"

//u_list's конструкторы и деструктор
template<typename T>
u_list<T>::u_list()
    : m_arr_size(0), m_start_node(nullptr), m_end_node(nullptr)
{      
    std::cout << "\n 1. Cоздан объект шаблонного класса u_list для хранения элементов размером " << sizeof(T) << " байта в контейнере типа «Двунаправленный список».";
}

template<typename T>
u_list<T>::u_list(u_list&& rhs) noexcept //доп. задание 3
{
    move_from(std::move(rhs));

    std::cout << "\n 1. Вызван конструктор перемещения u_list(u_list&& rhs)";
}

template<typename T>
u_list<T>& u_list<T>::operator=(u_list&& rhs) noexcept //доп. задание 3
{
    if (&rhs == this)
        return *this;

    move_from(std::move(rhs));

    std::cout << "\n 2. Вызван оператор присваивания перемещением operator=(u_list&& rhs)";
    return *this;
}

template<typename T>
u_list<T>::~u_list()
{        
    Node* it_delete = nullptr;

    for (Node* it = m_start_node; it != nullptr; )
    {
        it_delete = it;
        it = it->next;

        delete it_delete;
        it_delete = nullptr;
    }
}


//Node's конструктор
template<typename T>
u_list<T>::Node::Node()
    : m_data(0), next(nullptr), prev(nullptr)
{    
}

template<typename T>
u_list<T>::Node::Node(int data)
    : m_data(data), next(nullptr), prev(nullptr)
{
}


//u_list's public и friend функции
template<typename T>
void u_list<T>::push_back(const T& value)
{
    ++m_arr_size;

    Node* new_end_node = new Node{ value };

    if (m_arr_size == 1)
        m_start_node = m_end_node = new_end_node;
    else
    {        
       // new_end_node->m_data = value;
        new_end_node->prev = m_end_node;       
        m_end_node->next = new_end_node;       
        m_end_node = new_end_node;       
    }
}

template<typename T>
size_t u_list<T>::size() const
{
    return m_arr_size;
}

//@param element_pos > 0
template<typename T>
void u_list<T>::erase(const size_t element_pos)
{
    //проверяем позицию
    if (element_pos <= 0 || element_pos > m_arr_size)
        throw Errors::ErrorPosition;

    Node* erase_node = m_start_node;    

    //Поиск места для вставки
    for (size_t count_pos = 1; count_pos != element_pos; ++count_pos)
        erase_node = erase_node->next;

    //Меняем адреса для next и prev
    if (erase_node != m_start_node) //не первый
        erase_node->prev->next = erase_node->next;
    else
        m_start_node = erase_node->next;

    if (erase_node->next != nullptr) //не последний
        erase_node->next->prev = erase_node->prev;
    
    delete erase_node;
    erase_node = nullptr; 
    --m_arr_size;
}

//@param pos > 0
template<typename T>
void u_list<T>::insert(const size_t pos, const int value)
{
    if (pos < 1 || pos > m_arr_size)
        throw Errors::ErrorPosition;

    Node* old_node = m_start_node;
    Node* new_node = new Node{ value };
   
    if (pos == 1) //вставка на первое место
    {
        old_node->prev = m_start_node = new_node;
        new_node->next = old_node;
    }
    else if (pos > 1)
    {
        //Поиск места для вставки
        old_node = m_start_node->next;
        for (size_t count_pos = 2; count_pos != pos; ++count_pos) //старт со 2го нода       
            old_node = old_node->next;        

        //Новый адрес для предыдущего нода
        old_node->prev->next = new_node; 

        //prev и next для нового нода
        new_node->prev = old_node->prev;
        new_node->next = old_node;

        //Новый адрес для следующего нода
        old_node->prev = new_node; 
    }    

    ++m_arr_size;
}

template<typename T>
typename u_list<T>::iterator u_list<T>::begin()
{
    return iterator(m_start_node);
}

template<typename T>
typename u_list<T>::iterator u_list<T>::back()
{
    return iterator(m_end_node->prev);
}

template<typename T>
typename u_list<T>::iterator u_list<T>::end()
{
    return iterator(m_end_node->next);
}

template<typename T>
std::ostream& operator<<(std::ostream& out, u_list<T>& rhs)
{   
    if (rhs.size() == 0)
        return out;

    auto last = rhs.back();

    for(auto it = rhs.begin(); it != last; ++it) //доп. задание 4  
        out << *it << ", ";
    
    out << *last;
   
    return out;
}

//u_list's private функции
template<typename T>
void u_list<T>::move_from(u_list<T>&& rhs) noexcept {
    m_start_node = rhs.m_start_node;
    m_end_node = rhs.m_end_node;
    m_arr_size = rhs.m_arr_size;

    rhs.m_start_node = rhs.m_end_node = nullptr;
    rhs.m_arr_size = 0;
}


//===========================================================================================================
//доп. задание 4   
//iterator's конструкторы и public методы
template<typename T>
u_list<T>::iterator::iterator(Node* p)
    : m_iterator_ptr(p)
{}

template<typename T>
int& u_list<T>::iterator::operator*()
{    
    return m_iterator_ptr->m_data;
}

template<typename T>
typename u_list<T>::iterator& u_list<T>::iterator::operator++()
{
    m_iterator_ptr = m_iterator_ptr->next;
    return *this;
}

template<typename T>
bool u_list<T>::iterator::operator!=(typename u_list<T>::iterator& rhs)
{
    return  m_iterator_ptr != rhs.m_iterator_ptr;
}

template<typename T>
int& u_list<T>::iterator::get()
{
    return m_iterator_ptr->m_data;
}
