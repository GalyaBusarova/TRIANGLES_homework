#include <iostream>
#include <unordered_map>
#include <vector>
#include <limits>
#include <algorithm>

#include "geometry.hpp"

int main()
{
    int N = 0;
    std::cin >> N;

    std::unordered_map<int, std::vector<point_t<float>>> hash_of_triangles;

    float x, y, z;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            std::cin >> x >> y >> z;
            point_t<float> point(x, y, z);

            hash_of_triangles[i].push_back(point);
        }
    }

    std::vector<int> answer = find_answer(hash_of_triangles, N);

    std::sort(answer.begin(), answer.end());

    for (int i : answer)
    {
        std::cout << i << " ";
    }

    std::cout << std::endl;
}


