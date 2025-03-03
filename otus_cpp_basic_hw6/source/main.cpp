#include "main.h"

template<typename T>
void print(T& container, const int number, const std::string& name)
{
    std::cout << "\n " << number << ". Содержимое контейнера " << name << ": " << container;
}

template<typename T>
void print_move(T& container_moved, const int number, const std::string& name)
{
    std::cout << "\n " << number << ". Содержимое контейнера " << name << ": " << container_moved;
    std::cout << "\n-----------------------------------------------\n";
}

size_t middle_pos(size_t size)
{
    return (size / 2) + 1;
}

template<typename T>
void check_container(T& container, const int size_container, const std::string& name)
{    
    for (int i = 0; i < size_container; i++)
    {
        container.push_back(i);
    }
    
    std::cout << "\n 2. В контейнер "<< name << " добавлено " << size_container << " элементов.";
    print(container, 3, name);
    std::cout << "\n 4. Размер контейнера: " << container.size();
        
    container.erase(3);
    container.erase(4);
    container.erase(5);

    std::cout << "\n 5. Из контейнера удалены 3, 5 и 7 элементы.";
    print(container, 6, name);
    
    container.insert(1, 10);
    std::cout << "\n 7. Добавлен элемент 10 в начало контейнера.";
    print(container, 8, name);
    
    container.insert(middle_pos(container.size() ), 20);
    std::cout << "\n 9. Добавлен элемент 20 в середину контейнера.";
    print(container, 10, name);

    container.push_back(30);
    std::cout << "\n 11. Добавлен элемент 30 в конец контейнера.";
    print(container, 12, name);

    std::cout << "\n===============================================\n";
}

void check_move(u_vector<int>& vec, u_list<int>& list, u_forward_list& f_list)
{   
    std::cout << "\nДополнительное задание 3. Использовать в реализации семантику перемещения\n";

    u_vector<int> vec_2(std::move(vec));   
    print_move(vec_2, 2, "vec_2");

    u_vector<int> vec_3;
    vec_3 = static_cast<u_vector<int>&&>(vec_2); //Использую static_cast для практики
    print_move(vec_3,3, "vec_3");
    
    u_list<int> list_2(std::move(list));
    print_move(list_2, 2, "list_2");
    
    u_list<int> list_3;
    list_3 = std::move(list_2);
    print_move(list_3, 3, "list_3");
    
    u_forward_list f_list_2(std::move(f_list));
    print_move(f_list_2, 2, "f_list_2");

    u_forward_list f_list_3;
    f_list_3 = std::move(f_list_2);
    print_move(f_list_3,3, "f_list_3");
}

void getErrorMessage(Errors er)
{
	switch (er)
	{
	case Errors::NoError:
		break;
	case Errors::ErrorPosition:
		std::cerr << "\nError: позиция для erase/input < 1 ИЛИ > max size";
		break;
	case Errors::EmptyContainer:
		std::cerr << "\nError: Первый элемент контейнера пустой.";
		break;
    case Errors::OutOfBounds:
        std::cerr << "\nError: выход за границы массива";
        break;
	default:
		break;
	}
}

int main() 
{    
    std::locale::global(std::locale("en_US.utf8"));
    
    try {
       u_vector<int> vector{};
        check_container<u_vector<int>>(vector, 10, "vector");   //базовое + доп. задание 1

        u_list<int> list{};
        check_container<u_list<int>>(list, 13, "list");         //базовое

        u_forward_list f_list{};
        check_container<u_forward_list>(f_list, 18, "f_list");  //доп. задание 2

        check_move(vector, list, f_list); //доп. задание 3
    }
    catch (Errors er)
    {
        getErrorMessage(er);
    }

    //_CrtDumpMemoryLeaks(); — для проверки утечек памяти

    return 0;
}