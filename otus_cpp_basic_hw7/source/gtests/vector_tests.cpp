#include "tests_main.h"
#include "u_vector.h"

struct VectorTestF : public testing::Test
{
    //arrange
    u_vector<int> vector{};
    const size_t vector_start_size = 4;

    void SetUp() override
    {
        for (int i = 1; i <= vector_start_size; i++)
            vector.push_back(i);
    }
};

TEST_F(VectorTestF, create)
{
    //arrange
    const size_t expected = vector_start_size;

    //assert    
    EXPECT_EQ(vector.size(), expected);
}

TEST_F(VectorTestF, push_back)
{
    //arrange
    const int expected = 30;

    //act
    vector.push_back(expected);

    //assert    
    EXPECT_EQ(vector.back().get(), expected);
}

TEST_F(VectorTestF, insert_begin)
{
    //arrange 
    const int expected = 10;

    //act
    vector.insert(1, expected);

    //assert    
    EXPECT_EQ(vector.begin().get(), expected);
}

TEST_F(VectorTestF, insert_middle)
{
    //arrange 
    const int expected = 20;
    const size_t middle = (vector.size() / 2) + 1;

    //act
    vector.insert(middle, expected);

    u_vector<int>::iterator it = vector.begin();
    for (size_t count = 1; count != middle; ++count)
        ++it;

    //assert    
    EXPECT_EQ(it.get(), expected);
}

TEST_F(VectorTestF, erase_back)
{
    //arrange 
    const size_t expected = vector_start_size - 1;

    //act
    vector.erase(vector_start_size);

    //assert    
    EXPECT_EQ(vector.size(), expected);
}

TEST_F(VectorTestF, erase_begin)
{
    //arrange
    const size_t expected = vector_start_size - 1;

    //act
    vector.erase(1);

    //assert    
    EXPECT_EQ(vector.size(), expected);
}

TEST_F(VectorTestF, erase_middle)
{
    //arrange
    const size_t expected = vector_start_size - 1;
    const size_t middle = (vector.size() / 2) + 1;

    //act
    vector.erase(middle);

    //assert    
    EXPECT_EQ(vector.size(), expected);
}

TEST_F(VectorTestF, get_value)
{
    //arrange
    const int expected = 1;

    //act

    //assert    
    EXPECT_EQ(vector.begin().get(), expected);
}

TEST_F(VectorTestF, get_size)
{
    //arrange
    const size_t expected = vector_start_size;

    //act

    //assert    
    EXPECT_EQ(vector.size(), expected);
}

TEST_F(VectorTestF, extra1_copy)
{
    //arrange
    u_vector<int> vector2 = vector;
    bool expected_size = false;
    bool expected_arr = false;
    u_vector<int>::iterator it_vector = vector.begin();
    u_vector<int>::iterator it_vector2 = vector2.begin();

    //act
    if (vector2.size() == vector.size())
        expected_size = true;

    while (vector2.end() != it_vector2)
    {
        if (it_vector.get() != it_vector2.get())
        {
            expected_arr = false;
            break;
        }

        ++it_vector;
        ++it_vector2;
        expected_arr = true;
    }

    //assert    
    EXPECT_TRUE(expected_size);
    EXPECT_TRUE(expected_arr);
}

TEST_F(VectorTestF, extra3_move)
{
    //arange
    u_vector<int> vector2;
    int vector_begin = vector.begin().get();
    int vector_last = vector.back().get();

    //act
    vector2 = std::move(vector);    

    //assert
    EXPECT_EQ(vector_begin, vector2.begin().get());
    EXPECT_EQ(vector_last, vector2.back().get());

    EXPECT_EQ(0, vector.size());
}

