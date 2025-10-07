#include <iostream>
#include <vector>
#include <cassert>
#include <string>

#include "geometry.hpp"

struct TestCase
{
    std::string name;
    size_t tr_amount;
    std::vector<Triangle<float>> triangles;
    std::vector<int> answer;
};

void run_test(const TestCase& test_case)
{
    std::cout << test_case.name << " ";

    std::vector<int> answer = find_answer_with_grid(test_case.triangles);

    assert(answer == test_case.answer);
    std::cout << "is OK!" << std::endl;
}

void run_all_tests()
{
    std::vector<TestCase> test_cases = {
    {
        "test 1",
        2,
        {
            Triangle<float>({0, 0, 0}, {2, 0, 0}, {0, 2, 0}),
            Triangle<float>({5, 0, 0}, {7, 0, 0}, {5, 2, 0})
        },
        {}
    },
    
    {
        "test 2",
        2,
        {
            Triangle<float>({0, 0, 0}, {2, 0, 0}, {0, 2, 0}),
            Triangle<float>({1, 0, 0}, {3, 0, 0}, {1, 2, 0})
        },
        {0, 1}
    },

    {
        "test 3",
        2,
        {
            Triangle<float>({0, 0, 0}, {2, 0, 0}, {0, 2, 0}),
            Triangle<float>({0, 0, 1}, {2, 0, 1}, {0, 2, 1})
        },
        {}
    },

    {
        "test 4",
        2,
        {
            Triangle<float>({0, 0, 0}, {2, 0, 0}, {0, 2, 0}),
            Triangle<float>({0, 0, 0}, {0, 2, 0}, {0, 0, 2})
        },
        {0, 1}
    },

    {
        "test 5",
        2,
        {
            Triangle<float>({0, 0, 0}, {2, 0, 0}, {0, 2, 0}),
            Triangle<float>({0, 0, 0}, {0, 0, 2}, {0, 2, 0})
        },
        {0, 1}
    },

    {
        "test 6",
        5,
        {
            Triangle<float>({0, 0, 0}, {2, 0, 0}, {0, 2, 0}),   // 0
            Triangle<float>({1, 0, 0}, {3, 0, 0}, {1, 2, 0}),   // 1
            Triangle<float>({5, 0, 0}, {7, 0, 0}, {5, 2, 0}),   // 2
            Triangle<float>({0, 0, 5}, {2, 0, 5}, {0, 2, 5}),   // 3
            Triangle<float>({0, 0, 0}, {0, 2, 0}, {0, 0, 2})    // 4
        },
        {0, 1, 4}
    },

    {
        "test 7",
        2,
        {
            Triangle<float>({0, 0, 0}, {1, 0, 0}, {2, 0, 0}),   
            Triangle<float>({0, 1, 0}, {1, 1, 0}, {2, 1, 0})    
        },
        {}  
    },

    {
        "test 8",
        2,
        {
            Triangle<float>({0, 0, 0}, {1, 0, 0}, {0, 1, 0}),
            Triangle<float>({2, 0, 0}, {3, 0, 0}, {2, 1, 0})
        },
        {}
    }
    };

    for (const auto& test_case : test_cases) 
    {
        run_test(test_case);
    }
}

int main()
{
    run_all_tests();
}