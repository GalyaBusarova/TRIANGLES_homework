#include <iostream>
#include <unordered_map>
#include <vector>
#include <limits>
#include <algorithm>

#include "geometry.hpp"

bool check_the_edge_axes(std::vector<vect_t<float>> i_triangle, 
                        std::vector<vect_t<float>> j_triangle, 
                        std::unordered_map<int, std::vector<point_t<float>>> hash_of_triangles,
                        int tri_i_index, int tri_j_index);

bool is_valid_triangle(const std::vector<point_t<float>>& pts);

enum Parallel_Or_Not check_if_parallel(vect_t<float> normal_vec_first, vect_t<float> normal_vec_second, point_t<float> point_in_second, point_t<float> point_in_first);

bool check_the_axes_on_plane(std::vector<vect_t<float>> i_triangle, 
                        std::vector<vect_t<float>> j_triangle, 
                        std::unordered_map<int, std::vector<point_t<float>>> hash_of_triangles,
                        int tri_i_index, int tri_j_index, 
                        vect_t<float> normal_vect);

int main()
{
    int N = 0;
    std::cout << "Введите число треугольников: ";
    std::cin >> N;

    std::unordered_map<int, std::vector<point_t<float>>> hash_of_triangles;

    float x, y, z;

    for (int i = 0; i < N; i++)
    {
        std::cout << "Введите координаты " << i + 1 << " треугольника: ";
        for (int j = 0; j < 3; j++)
        {
            std::cin >> x >> y >> z;
            point_t<float> point(x, y, z);

            hash_of_triangles[i].push_back(point);
        }
    }

    std::vector<int> answer;

    for (int i = 0; i < N; i++)
    {
        vect_t<float> first_vec_i = hash_of_triangles[i][0].create_vector_between(hash_of_triangles[i][1]);
        vect_t<float> second_vec_i = hash_of_triangles[i][1].create_vector_between(hash_of_triangles[i][2]);
        vect_t<float> third_vec_i = hash_of_triangles[i][2].create_vector_between(hash_of_triangles[i][0]);

        if (first_vec_i.check_if_null() || second_vec_i.check_if_null() || third_vec_i.check_if_null())
        {
            continue;
        }

        std::vector<vect_t<float>> i_triangle = {first_vec_i, second_vec_i, third_vec_i};
        if (!is_valid_triangle(hash_of_triangles[i])) 
        {
            continue;
        }
        
        for (int j = i + 1; j < N; j++)
        {
            vect_t<float> first_vec_j = hash_of_triangles[j][0].create_vector_between(hash_of_triangles[j][1]);
            vect_t<float> second_vec_j = hash_of_triangles[j][1].create_vector_between(hash_of_triangles[j][2]);
            vect_t<float> third_vec_j = hash_of_triangles[j][2].create_vector_between(hash_of_triangles[j][0]);

            if (first_vec_j.check_if_null() || second_vec_j.check_if_null() || third_vec_j.check_if_null())
            {
                continue;
            }

            std::vector<vect_t<float>> j_triangle = {first_vec_j, second_vec_j, third_vec_j};
            if (!is_valid_triangle(hash_of_triangles[j]))
            {
                continue;
            }

            vect_t<float> normal_vec_i = first_vec_i.vect_compos(second_vec_i);
            vect_t<float> normal_vec_j = first_vec_j.vect_compos(second_vec_j);

            Parallel_Or_Not status = check_if_parallel(normal_vec_i, normal_vec_j, hash_of_triangles[j][0], hash_of_triangles[i][0]);

            if (status == IN_THE_SAME_PLANE)
            {
                if (check_the_axes_on_plane(i_triangle, j_triangle, hash_of_triangles, i, j, normal_vec_i))
                {
                    if (std::find(answer.begin(), answer.end(), i) == answer.end())
                    {
                        answer.push_back(i);
                    }
                    if (std::find(answer.begin(), answer.end(), j) == answer.end())
                    {
                        answer.push_back(j);
                    }
                }

                else
                {
                    continue;
                }
            }

            if (status == IN_PARALLEL_PLANES)
            {
                continue;
            }

            int summ_i = 0;

            point_t<float> refer_point_j = hash_of_triangles[j][0];

            for (int k = 0; k < 3; k++)
            {
                if (normal_vec_j.scalar_compos(refer_point_j.create_vector_between(hash_of_triangles[i][k])) < 0){summ_i--;}
                if (normal_vec_j.scalar_compos(refer_point_j.create_vector_between(hash_of_triangles[i][k])) > 0){summ_i++;}
            }

            if (summ_i == 3 || summ_i == -3)
            {
                continue;
            }

            int summ_j = 0;

            point_t<float> refer_point_i = hash_of_triangles[i][0];

            for (int k = 0; k < 3; k++)
            {
                if (normal_vec_i.scalar_compos(refer_point_i.create_vector_between(hash_of_triangles[j][k])) < 0){summ_j--;}
                if (normal_vec_i.scalar_compos(refer_point_i.create_vector_between(hash_of_triangles[j][k])) > 0){summ_j++;}
            }

            if (summ_j == 3 || summ_j == -3)
            {
                continue;
            }

            else
            {
                if (check_the_edge_axes(i_triangle, j_triangle, hash_of_triangles, i, j))
                {
                    if (std::find(answer.begin(), answer.end(), i) == answer.end())
                    {
                        answer.push_back(i);
                    }
                    if (std::find(answer.begin(), answer.end(), j) == answer.end())
                    {
                        answer.push_back(j);
                    }
                }

                else
                {
                    continue;
                }
            }
        }
    }

    std::sort(answer.begin(), answer.end());

    for (int i : answer)
    {
        std::cout << i << " ";
    }

    std::cout << std::endl;
}


bool check_the_edge_axes(std::vector<vect_t<float>> i_triangle, 
                        std::vector<vect_t<float>> j_triangle, 
                        std::unordered_map<int, std::vector<point_t<float>>> hash_of_triangles,
                        int tri_i_index, int tri_j_index)  
{
    for (int edge_i = 0; edge_i < 3; edge_i++) {
        for (int edge_j = 0; edge_j < 3; edge_j++) {
            
            vect_t<float> normal_vect_axe = i_triangle[edge_i].vect_compos(j_triangle[edge_j]);
            
            if (normal_vect_axe.getLength() < EPSILON) 
            {
                continue; 
            }
            vect_t<float> normalized_axis = normal_vect_axe.normalise();
            
            float min_proj_i = std::numeric_limits<float>::max();
            float max_proj_i = std::numeric_limits<float>::lowest();
            
            for (int k = 0; k < 3; k++) {
                float proj = hash_of_triangles[tri_i_index][k].project_point(normalized_axis);
                min_proj_i = std::min(min_proj_i, proj);
                max_proj_i = std::max(max_proj_i, proj);
            }
              
            float min_proj_j = std::numeric_limits<float>::max();
            float max_proj_j = std::numeric_limits<float>::lowest();
            
            for (int k = 0; k < 3; k++) {
                float proj = hash_of_triangles[tri_j_index][k].project_point(normalized_axis);
                min_proj_j = std::min(min_proj_j, proj);
                max_proj_j = std::max(max_proj_j, proj);
            }
            
            if (max_proj_i < min_proj_j || max_proj_j < min_proj_i) {
                return false; 
            }
        }
    }
    return true;
}

// checks whether a set of vectors is a triangle
bool is_valid_triangle(const std::vector<point_t<float>>& pts) {
    vect_t<float> v1 = pts[0].create_vector_between(pts[1]);
    vect_t<float> v2 = pts[0].create_vector_between(pts[2]);
    vect_t<float> n = v1.vect_compos(v2);
    return n.getLength() >= EPSILON;
}


// check if the triangles are in parallel planes
enum Parallel_Or_Not check_if_parallel(vect_t<float> normal_vec_first, vect_t<float> normal_vec_second, point_t<float> point_in_second, point_t<float> point_in_first)
{
    vect_t<float> cross = normal_vec_first.vect_compos(normal_vec_second);
    if (cross.getLength() < EPSILON) 
    {
        float d_first = -(normal_vec_first.getX() * point_in_first.getX() +
                        normal_vec_first.getY() * point_in_first.getY() +
                        normal_vec_first.getZ() * point_in_first.getZ());

        float dist = (normal_vec_first.getX() * point_in_second.getX() +
                    normal_vec_first.getY() * point_in_second.getY() +
                    normal_vec_first.getZ() * point_in_second.getZ() + d_first);

        if (std::abs(dist) < EPSILON) 
        {
            return IN_THE_SAME_PLANE;
        } 

        else 
        {
            return IN_PARALLEL_PLANES;
        }
    } 
    else 
    {
        return NOT_PARALLEL;
    }
}


bool check_the_axes_on_plane(std::vector<vect_t<float>> i_triangle, 
                        std::vector<vect_t<float>> j_triangle, 
                        std::unordered_map<int, std::vector<point_t<float>>> hash_of_triangles,
                        int tri_i_index, int tri_j_index, 
                        vect_t<float> normal_vect)
{
    for (int i = 0; i < 3; i++)
    {
        vect_t<float> temp = normal_vect.vect_compos(i_triangle[i]);
        if (temp.getLength() < EPSILON) continue;
        vect_t<float> axe = temp.normalise();

        float min_proj_i = std::numeric_limits<float>::max();
        float max_proj_i = std::numeric_limits<float>::lowest();

        for (int k = 0; k < 3; k++) 
        {
            float proj = hash_of_triangles[tri_i_index][k].project_point(axe);
            min_proj_i = std::min(min_proj_i, proj);
            max_proj_i = std::max(max_proj_i, proj);
        }

        float min_proj_j = std::numeric_limits<float>::max();
        float max_proj_j = std::numeric_limits<float>::lowest();
            
        for (int k = 0; k < 3; k++) 
        {
            float proj = hash_of_triangles[tri_j_index][k].project_point(axe);
            min_proj_j = std::min(min_proj_j, proj);
            max_proj_j = std::max(max_proj_j, proj);
        }

        if (max_proj_i < min_proj_j || max_proj_j < min_proj_i) 
        {
            return false; 
        }
    }

    for (int j = 0; j < 3; j++)
    {
        vect_t<float> temp = normal_vect.vect_compos(j_triangle[j]);
        if (temp.getLength() < EPSILON) continue;
        vect_t<float> axe = temp.normalise();

        float min_proj_i = std::numeric_limits<float>::max();
        float max_proj_i = std::numeric_limits<float>::lowest();

        for (int k = 0; k < 3; k++) 
        {
            float proj = hash_of_triangles[tri_i_index][k].project_point(axe);
            min_proj_i = std::min(min_proj_i, proj);
            max_proj_i = std::max(max_proj_i, proj);
        }

        float min_proj_j = std::numeric_limits<float>::max();
        float max_proj_j = std::numeric_limits<float>::lowest();
            
        for (int k = 0; k < 3; k++) 
        {
            float proj = hash_of_triangles[tri_j_index][k].project_point(axe);
            min_proj_j = std::min(min_proj_j, proj);
            max_proj_j = std::max(max_proj_j, proj);
        }

        if (max_proj_i < min_proj_j || max_proj_j < min_proj_i) 
        {
            return false; 
        }
    }

    return true;
}






// ок
// два непересекающихся треугольника - неверно
// ок
// ок
// ok
// 5 треугольников - неверно
// ок
// треугольники в одной плоскости, но не пересекаются - неверно
// ok
// ок