#include "main.h"

//Base task
int max_number = 100;
const int min_number = 0;
constexpr int max_int_size = std::numeric_limits<int>::max();
constexpr char max_char_size = std::numeric_limits<char>::max();

const std::string file_path("file_result.txt");


void Compare(int &attempt_count, const int &max_number)
{
    const int random_number = Get_Some_Number(max_number);
    int user_value = -1; //it is error value, need >= 0
  
    for (attempt_count = 0; user_value != random_number; attempt_count++)
    {
        if (attempt_count > 0)
        {
            if (user_value > random_number)           
                std::cout << user_value  << " is greater. Try to guess again: ";

            if (user_value < random_number)
                std::cout << user_value << " is less. Try to guess again: ";
        }

        Get_User_Value(user_value, max_number);
    }
}

void Save(std::string &user_name, int &attempt_count)
{
    std::ofstream file(file_path, std::ios::app);

    if (file.is_open() == false)
    {
        std::cout << "Error. The file is not open. Your result not saved.";
        return;
    }

    file << user_name << "\t" << attempt_count << std::endl;
    file.close();
}


void Get_User_Name(std::string &user_name)
{
    std::cout << "Please, enter your name: ";
    std::cin >> user_name;
}

int Get_Some_Number(const int &max_number)
{
    int some_number = std::rand() % max_number;
    
    return some_number;
}

void Get_User_Value(int &user_value, const int &max_number)
{
    std::cin >> user_value;

    while (std::cin.fail() || (user_value > max_number) || (user_value < min_number) )
    {
        std::cin.clear();
        std::cin.ignore(max_int_size, '\n');
        std::cout << "Error. Enter integer from 0 untill " << max_number << ": ";
        std::cin >> user_value;
    }
}
 
void View_List_Results() // This function uses only with argument "-table" beacause there is a extra-task function "View_and_Save_Min_Result()"
{
    std::string input;

    std::ifstream file(file_path);
    file.seekg(0);

    std::cout << "\nHigh scores table\n" << "\nName\t" << "Scores" << std::endl;  
    std::getline(file, input, '$');
    std::cout << input;

    file.close();
}
//-----------------------------------------------------------------------------------------------------


//Extra tasks: 
// 1, 2, 5

void Treat_Arguments(int& argc, char** argv)
{
    //extra 5
    for (int i = 1; i < argc; i++)
    {
        std::string parametr = argv[i];

        if (parametr == "-max" || parametr == "-level")
        {
            for (int j = 2; j < argc; j++)
            {
                std::string double_parametr = argv[j];

                if (double_parametr == "-max" || double_parametr == "-level")
                {
                    std::cout << "Error. Set only one parametr: '-max' OR '-level' " << std::endl;
                    exit(0);
                }
            }
        }

        Treat_Arguments(i, parametr, argv);
    }
}

void Treat_Arguments(int i, std::string parametr, char** argv)
{
    //extra 1
    if (parametr == "-max")
    {
        try
        {
            if (argv[i + 1] == NULL)
                throw std::exception("The argument '-max' requires some integer > 0 (example: -max 50). The max has been autoset.");

            max_number = std::stoi(argv[i + 1]); //convertation from char to integer
        }
        catch (const std::exception& ex)
        {
            std::cout << "Error: " << ex.what() << std::endl;
            return;
        }

        return;
    }

    //extra 5
    if (parametr == "-level")
    {
        int level = 0;

        try
        {
            if (argv[i + 1] == NULL)
                throw std::exception("The argument '-level' requires 1, 2 or 3. Example: -level 2");

            level = std::stoi(argv[i + 1]); //convertation from char to integer
        }
        catch (const std::exception& ex)
        {
            std::cout << "Error: " << ex.what() << std::endl;
            return;
        }

        switch (level)
        {

        case 1:
            max_number = 9;
            break;
        case 2:
            max_number = 49;
            break;
        case 3:
            max_number = 99;
            break;
        default:
            break;
        }

        return;
    }
    
    //extra 2
    if (parametr == "-table")
    {
        View_List_Results();
        std::cout << std::endl;
        exit(0);
    }
}

// 3, 4
void View_and_Save_Min_Result()
{
    // preparation: GET vector of name  and vector of results from file

    std::vector<std::string> user_names;
    std::vector<int> user_results;

    std::fstream file;

    file.open(file_path, std::ios::in);

    if (!file.is_open())
    {
        std::cout << "Error. The save-file is not open. You can't view result table.";
        return;
    }

    file.seekg(0); 
    Fill_Two_Vectors(file, user_names, user_results);
    file.close();

    // VIEW table and SAVE result

    file.open(file_path, std::ios::out | std::ios::trunc);
    Print_and_Save_Min_Result( file, user_names, user_results);   
    file.close();
}

void Fill_Two_Vectors(std::fstream& file, std::vector<std::string>& user_names, std::vector<int>& user_results)
{
    int i = 0;
    int max_count_name = 0;
    bool is_first_iteration = true;
    std::string name_i, result_i;
    int result;

    while (!file.eof())
    {
        std::getline(file, name_i, '\t');

        std::getline(file, result_i, '\n');
        result = std::stoi(result_i);

        if (is_first_iteration)
        {
            user_names.push_back(name_i);
            user_results.push_back(result);
            is_first_iteration = false;
            ++i;
            continue;
        }

        max_count_name = (int)user_names.size() - 1;
        for (int j = 0; j <= max_count_name; j++)
        {
            if (name_i == user_names[j])
            {
                if (result < user_results[j])
                    user_results[j] = result;

                break;
            }

            if (j == max_count_name)
            {
                user_names.push_back(name_i);
                user_results.push_back(result);
                ++i;
                break;
            }
        }
    }

    //clear last excess string (this string contains '\n' because of the use of Save() )
    user_names.pop_back();
    user_results.pop_back();
}

void Print_and_Save_Min_Result(std::fstream& file, std::vector<std::string>& user_names, std::vector<int>& user_results)
{
    std::cout << "----\t" << "-----------"
        << "\nName\t" << "Best scores\n"
        << "----\t" << "-----------" << std::endl;

    for (int l = 0; l < user_names.size(); l++)
    {
        std::cout << user_names[l] << '\t' << user_results[l] << std::endl;
        file << user_names[l] << '\t' << user_results[l] << std::endl;
    }

}

//My
bool Play_Again()
{
    char user_chois;

    std::cout << "Do you want to play again? (y/n):  ";
    std::cin >> user_chois;

    while (std::cin.fail() || ((int)user_chois != 121 && (int)user_chois != 89 && (int)user_chois != 110 && (int)user_chois != 78))
    {
        std::cin.clear();
        std::cin.ignore(max_char_size, '\n');
        std::cout << "Error. Please, enter \"y\" or \"n\": ";
        std::cin >> user_chois;
    }

    if ((int)user_chois == 121 || (int)user_chois == 89) // 121 = y, 89 = Y
        return true;
    if ((int)user_chois == 110 || (int)user_chois == 78) // 110 = n, 78 = N
        return false;

    return false;
}
//-----------------------------------------------------------------------------------------------------



int main(int argc, char *argv[])
{   
    Treat_Arguments(argc, argv); //extra task
   
    std::srand((unsigned int)std::time(nullptr));

    std::string user_name;
    int attempt_count = 0;

    Get_User_Name(user_name);

    std::cout << "\n***** START GAME *****" << std::endl;

    do
    {
        std::cout << "\nThe computer thinking of a number from 0 to " << max_number << ". Guess and enter this number: ";

        Compare(attempt_count, max_number);
        Save(user_name, attempt_count);
        std::cout << "You Win! Your attempt = " << attempt_count << std::endl;

    } while (Play_Again() );

    View_and_Save_Min_Result(); //extra task
       
    std::cout << "\n***** END GAME *****\n" << std::endl;

    return 0;
}
