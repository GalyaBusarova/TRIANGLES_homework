#include <iostream>
#include <vector>
#include <cassert>
#include <string>
#include <fstream>
#include <sstream>

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
        "test 1 - intersecting triangles - ",
        2,
        {
            Triangle<float>({0, 0, 0}, {2, 0, 0}, {0, 2, 0}),
            Triangle<float>({5, 0, 0}, {7, 0, 0}, {5, 2, 0})
        },
        {}
    },
    
    {
        "test 2 - non-intersecting triangles - ",
        2,
        {
            Triangle<float>({0, 0, 0}, {2, 0, 0}, {0, 2, 0}),
            Triangle<float>({1, 0, 0}, {3, 0, 0}, {1, 2, 0})
        },
        {0, 1}
    },

    {
        "test 3 - triangles in parallel planes - ",
        2,
        {
            Triangle<float>({0, 0, 0}, {2, 0, 0}, {0, 2, 0}),
            Triangle<float>({0, 0, 1}, {2, 0, 1}, {0, 2, 1})
        },
        {}
    },

    {
        "test 4 - triangles touch along the edge - ",
        2,
        {
            Triangle<float>({0, 0, 0}, {2, 0, 0}, {0, 2, 0}),
            Triangle<float>({0, 0, 0}, {0, 2, 0}, {0, 0, 2})
        },
        {0, 1}
    },

    {
        "test 5 - the triangles intersect at one point - ",
        2,
        {
            Triangle<float>({0, 0, 0}, {2, 0, 0}, {0, 2, 0}),
            Triangle<float>({0, 0, 0}, {0, 0, 2}, {0, 2, 0})
        },
        {0, 1}
    },

    {
        "test 6 - 5 triangles - ",
        5,
        {
            Triangle<float>({0, 0, 0}, {2, 0, 0}, {0, 2, 0}),   
            Triangle<float>({1, 0, 0}, {3, 0, 0}, {1, 2, 0}),   
            Triangle<float>({5, 0, 0}, {7, 0, 0}, {5, 2, 0}),   
            Triangle<float>({0, 0, 5}, {2, 0, 5}, {0, 2, 5}),   
            Triangle<float>({0, 0, 0}, {0, 2, 0}, {0, 0, 2})    
        },
        {0, 1, 4}
    },

    {
        "test 7 - degenerate triangle (three points on one line) - ",
        2,
        {
            Triangle<float>({0, 0, 0}, {1, 0, 0}, {2, 0, 0}),   
            Triangle<float>({0, 1, 0}, {1, 1, 0}, {2, 1, 0})    
        },
        {}  
    },

    {
        "test 8 - triangles in the same plane, but do not intersect - ",
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
    
    std::cout << "All tests passed!" << std::endl;
}


std::vector<Triangle<float>> read_triangles_from_file(const std::string& filename) {
    std::vector<Triangle<float>> triangles;
    std::ifstream file(filename);
    
    if (!file.is_open()) 
    {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    int triangle_count;
    file >> triangle_count;
    
    for (int i = 0; i < triangle_count; ++i) 
    {
        float x1, y1, z1, x2, y2, z2, x3, y3, z3;
        
        file >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
        
        if (file.fail()) {
            throw std::runtime_error("Failed to read coordinates for triangle " + std::to_string(i));
        }
        
        point_t<float> p1 = {x1, y1, z1};
        point_t<float> p2 = {x2, y2, z2};
        point_t<float> p3 = {x3, y3, z3};
        
        triangles.push_back(Triangle<float>(p1, p2, p3));
    }
    
    file.close();
    return triangles;
}


void run_tests_from_file(const std::string& filename)
{
    std::vector<std::string> possible_paths = {
        filename,                          
        "../" + filename,                  
        "../../" + filename
    };
    
    std::string found_path;
    std::ifstream test_file;
    
    for (const auto& path : possible_paths) 
    {
        test_file.open(path);
        if (test_file.is_open()) {
            found_path = path;
            break;
        }
        test_file.close();
    }
    
    if (found_path.empty()) {
        std::cerr << "Cannot find file: " << filename << std::endl;
        return;
    }
    
    try 
    {
        std::vector<Triangle<float>> triangles = read_triangles_from_file(found_path);
        
        if (triangles.empty()) 
        {
            std::cout << "No triangles found in file" << std::endl;
            return;
        }
        
        std::vector<int> result = find_answer_with_grid(triangles);
        
        std::string output_path = "../big_test_output.txt";

        std::ofstream output_file(output_path);
        for (size_t i = 0; i < result.size(); ++i) 
        {
            output_file << result[i] << std::endl;

        }
        output_file.close();

        std::cout << "Result saved to: " << output_path << std::endl;
        std::cout << "File location: project folder" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error reading file: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc == 1) 
    {
        run_all_tests();
    }
    else if (argc == 2) 
    {
        std::string argument = argv[1];
        
        run_tests_from_file(argument);
    }
    else 
    {
        std::cerr << "Too many arguments!" << std::endl;
        return 1;
    }
    
    return 0;
}