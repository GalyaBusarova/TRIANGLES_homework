#include <iostream>
#include <unordered_map>
#include <vector>
#include <limits>
#include <algorithm>
#include <array>

#include "geometry.hpp"

int main()
{
    int N = 0;
    std::cin >> N;

    std::vector<Triangle<float>> triangles;

    float x, y, z;

    for (int i = 0; i < N; i++)
    {
        std::cin >> x >> y >> z;
        point_t<float> point1(x, y, z);

        std::cin >> x >> y >> z;
        point_t<float> point2(x, y, z);

        std::cin >> x >> y >> z;
        point_t<float> point3(x, y, z);

        Triangle<float> tr_i(point1, point2, point3);
        triangles.push_back(tr_i);
    }

    std::vector<int> answer = find_answer_with_grid(triangles);

    for (int i : answer)
    {
        std::cout << i << " ";
    }

    std::cout << std::endl;
}


