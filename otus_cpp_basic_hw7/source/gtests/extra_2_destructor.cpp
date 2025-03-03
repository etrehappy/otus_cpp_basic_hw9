#include "tests_main.h"
#include "u_vector.h"

//just for practice, extra2
struct MockVector : public u_vector<int> 
{
    MOCK_METHOD0(DieMock, int());

    ~MockVector() override
    {
        DieMock();
    }
};

//just for practic, extra2
TEST(MockPractice, vector_destructor)
{
    //Arrange   
    MockVector mock_vector;

    for (int i = 1; i <= 4; i++)
        mock_vector.push_back(i);

    //Prepare mocks
    EXPECT_CALL(mock_vector, DieMock()).Times(1);
}
