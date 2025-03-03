#include "tests_main.h"
#include "u_list.h"


struct ListTestF : public testing::Test
{
    //arrange
    u_list<int> list{};
    const size_t list_start_size = 4;

    void SetUp() override
    {
        for (int i = 1; i <= list_start_size; i++)
            list.push_back(i);
    }
};

TEST_F(ListTestF, create)
{
    //arrange
    const size_t expected = list_start_size;

    //assert    
    EXPECT_EQ(list.size(), expected);
}

TEST_F(ListTestF, push_back)
{
    //arrange
    const int expected = 30;

    //act
    list.push_back(expected);    

    //assert    
    EXPECT_EQ(list.back().get(), expected);
}

TEST_F(ListTestF, insert_begin)
{
    //arrange 
    const int expected = 10;

    //act
    list.insert(1, expected);

    //assert    
    EXPECT_EQ(list.begin().get(), expected);
}

TEST_F(ListTestF, insert_middle)
{
    //arrange 
    const int expected = 20;
    const size_t middle = (list.size() / 2) + 1;

    //act
    list.insert(middle, expected);

    u_list<int>::iterator list_it = list.begin();
    for (size_t count = 1; count != middle; ++count)
        ++list_it;

    //assert    
    EXPECT_EQ(list_it.get(), expected);
}


TEST_F(ListTestF, erase_back)
{
    //arrange 
    const size_t expected = list_start_size - 1;

    //act
    list.erase(list_start_size);

    //assert    
    EXPECT_EQ(list.size(), expected);
}

TEST_F(ListTestF, erase_begin)
{
    //arrange
    const size_t expected = list_start_size - 1;

    //act
    list.erase(1);

    //assert    
    EXPECT_EQ(list.size(), expected);
}

TEST_F(ListTestF, erase_middle)
{
    //arrange
    const size_t expected = list_start_size - 1;
    const size_t middle = (list.size() / 2) + 1;

    //act
    list.erase(middle);

    //assert    
    EXPECT_EQ(list.size(), expected);
}


TEST_F(ListTestF, get_value)
{
    //arrange
    const int expected = 1;

    //act

    //assert    
    EXPECT_EQ(list.begin().get(), expected);
}

TEST_F(ListTestF, get_size)
{
    //arrange
    const size_t expected = list_start_size;

    //act

    //assert    
    EXPECT_EQ(list.size(), expected);
}

TEST_F(ListTestF, extra1_copy)
{
    //arrange
    u_list<int> list2 = list;
    bool expected_size = false;
    bool expected_arr = false;
    u_list<int>::iterator it_list = list.begin();
    u_list<int>::iterator it_list2 = list2.begin();

    //act
    if (list2.size() == list.size())
        expected_size = true;

    while (list2.end() != it_list2)
    {
        if (it_list.get() != it_list2.get())
        {
            expected_arr = false;
            break;
        }

        ++it_list;
        ++it_list2;
        expected_arr = true;
    }

    //assert    
    EXPECT_TRUE(expected_size);
    EXPECT_TRUE(expected_arr);
}

TEST_F(ListTestF, extra3_move)
{
    //arange
    u_list<int> list2;  
    int list_begin = list.begin().get();
    int list_last = list.back().get();

    //act
    list2 = std::move(list);

    //assert
    EXPECT_EQ(list_begin, list2.begin().get());
    EXPECT_EQ(list_last, list2.back().get());

    EXPECT_EQ(0, list.size());
}


