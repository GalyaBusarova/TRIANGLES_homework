#pragma once

#include <unordered_set>
#include <map>
#include <tuple>        
#include <array>       
#include <algorithm>    
#include <cmath>        
#include <limits>       

const float EPSILON = 1e-6f;

enum Parallel_Or_Not
{
    NOT_PARALLEL,
    IN_THE_SAME_PLANE,
    IN_PARALLEL_PLANES
};



// vect struct
template <typename T> class vect_t
{
    T x, y, z;

public:
    vect_t(T x, T y, T z) : x(x), y(y), z(z) {}

    // A function that calculates the scalar product
    float scalar_compos(const vect_t v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }

    vect_t normalise() const 
    {
        float len = getLength();
        if (len < EPSILON) return vect_t(0, 0, 0);
        return vect_t(x / len, y / len, z / len);
    }

    // A function that calculates the vector product
    vect_t vect_compos(const vect_t v) const
    {
        vect_t<T> result(0, 0, 0);

        result.x = y * v.z - z * v.y;
        result.y = z * v.x - x * v.z;
        result.z = x * v.y - y * v.x;

        return result;
    }

    // A function that calculates the projection of a vector vect onto a vector line_vect
    vect_t proj_to_line(const vect_t line_vect) const
    {
        float scalar = scalar_compos(line_vect);
        float denominator = line_vect.length * line_vect.length;
        
        if (denominator == 0) return vect_t(0, 0, 0); 
        
        float factor = scalar / denominator;
        return vect_t(
            factor * line_vect.x,
            factor * line_vect.y,
            factor * line_vect.z
        );
    }

    // operator == to compare vectors
    bool operator==(const vect_t& other) const 
    {
        vect_t<T> dif_res (x - other.getX(), y - other.getY(), z - other.getZ());
        return dif_res.check_if_null();
    }

    //checks whether two points of a triangle are equal
    bool check_if_null() const
    {
        if (fabs(x) < EPSILON && fabs(y) < EPSILON && fabs(z) < EPSILON)
        {
            return true;
        }

        return false;
    }


    //геттеры
    float getLength() const 
    {
        return sqrtf(x*x + y*y + z*z);
    }
    T getX() const {return x;}
    T getY() const {return y;}
    T getZ() const {return z;};
};


template <typename T> struct point_t
{
    T x, y, z;

public:
    point_t(T x, T y, T z) : x(x), y(y), z(z) {};

    vect_t<T> create_vector_between(const point_t& other) const 
    {
        return vect_t<T>(other.x - x, other.y - y, other.z - z);
    }

    T project_point(const vect_t<float> axis) const
    {
        return x * axis.getX() + 
            y * axis.getY() + 
            z * axis.getZ();
    }

    //геттеры
    T getX() const {return x;}
    T getY() const {return y;}
    T getZ() const {return z;};
};


// check if the triangles are in parallel planes
enum Parallel_Or_Not check_if_parallel(const vect_t<float>& normal_vec_first, const vect_t<float>& normal_vec_second, const point_t<float>& point_in_second, const point_t<float>& point_in_first)
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


template <typename T> class Triangle 
{
    std::array<point_t<T>, 3> points;

public:
    // ctor
    Triangle(const point_t<T>& p0, const point_t<T>& p1, const point_t<T>& p2)
        : points{p0, p1, p2} {}

    // check is valid or not
    bool is_valid() const 
    {
        auto v1 = points[0].create_vector_between(points[1]);
        auto v2 = points[0].create_vector_between(points[2]);
        return v1.vect_compos(v2).getLength() >= EPSILON;
    }

    // return a normal vector
    vect_t<T> get_normal() const 
    {
        auto v1 = points[0].create_vector_between(points[1]);
        auto v2 = points[0].create_vector_between(points[2]);
        return v1.vect_compos(v2);
    }

    // возвращает три стороны
    std::array<vect_t<T>, 3> get_edges() const 
    {
        return {
            points[0].create_vector_between(points[1]),
            points[1].create_vector_between(points[2]),
            points[2].create_vector_between(points[0])
        };
    }

    // find a proj to line
    std::pair<T, T> proj_to_axe(const vect_t<T>& axis) const 
    {
        T min_proj = std::numeric_limits<T>::max();
        T max_proj = std::numeric_limits<T>::lowest();

        for (const auto& p : points) 
        {
            T proj = p.project_point(axis);

            min_proj = std::min(min_proj, proj);
            max_proj = std::max(max_proj, proj);
        }
        return {min_proj, max_proj};
    }

    // строит сетку для одного треугольника
    std::pair<point_t<T>, point_t<T>> get_bound_box() const 
    {
        float min_x = std::min({points[0].getX(), points[1].getX(), points[2].getX()});
        float min_y = std::min({points[0].getY(), points[1].getY(), points[2].getY()});
        float min_z = std::min({points[0].getZ(), points[1].getZ(), points[2].getZ()});

        float max_x = std::max({points[0].getX(), points[1].getX(), points[2].getX()});
        float max_y = std::max({points[0].getY(), points[1].getY(), points[2].getY()});
        float max_z = std::max({points[0].getZ(), points[1].getZ(), points[2].getZ()});

        return {point_t<float>(min_x, min_y, min_z),
                point_t<float>(max_x, max_y, max_z)};
    }

    // геттер
    const std::array<point_t<T>, 3>& get_points() const { return points; }
};


template <typename T>
bool check_the_edge_axes(
    const std::array<vect_t<T>, 3>& edges_a,
    const std::array<vect_t<T>, 3>& edges_b,
    const Triangle<T>& tri_a,
    const Triangle<T>& tri_b)
{
    for (int edge_i = 0; edge_i < 3; ++edge_i) 
    {
        for (int edge_j = 0; edge_j < 3; ++edge_j) 
        {
            vect_t<T> cross_axis = edges_a[edge_i].vect_compos(edges_b[edge_j]);
            
            if (cross_axis.getLength() < EPSILON) 
            {
                continue; 
            }

            vect_t<T> axis = cross_axis.normalise();
            
            auto proj_a = tri_a.proj_to_axe(axis);
            auto proj_b = tri_b.proj_to_axe(axis);
            
            if (proj_a.second < proj_b.first || proj_b.second < proj_a.first) 
            {
                return false; 
            }
        }
    }
    return true;
}

template <typename T> 
bool check_the_axes_on_plane(const std::array<vect_t<T>, 3>& edges_a,
                            const std::array<vect_t<T>, 3>& edges_b,
                            const Triangle<T>& tri_a,
                            const Triangle<T>& tri_b, 
                            const vect_t<T>& normal_vect)
{
    for (int i = 0; i < 3; i++)
    {
        vect_t<T> temp = normal_vect.vect_compos(edges_a[i]);
        if (temp.getLength() < EPSILON) continue;
        vect_t<T> axe = temp.normalise();

        std::pair<T, T> proj_a = tri_a.proj_to_axe(axe);
            
        std::pair<T, T> proj_b = tri_b.proj_to_axe(axe);

        if (proj_a.second < proj_b.first || proj_b.second < proj_a.first) 
        {
            return false; 
        }
    }

    for (int j = 0; j < 3; j++)
    {
        vect_t<T> temp = normal_vect.vect_compos(edges_b[j]);
        if (temp.getLength() < EPSILON) continue;
        vect_t<T> axe = temp.normalise();

        std::pair<T, T> proj_a = tri_a.proj_to_axe(axe);

        std::pair<T, T> proj_b = tri_b.proj_to_axe(axe);

        if (proj_a.second < proj_b.first || proj_b.second < proj_a.first) 
        {
            return false; 
        }
    }

    return true;
}

template <typename T> 
vect_t<T> create_vector_between(const point_t<T> from, const point_t<T> to) 
{
    return vect_t<T>(to.getX() - from.getX(), 
                    to.getY() - from.getY(), 
                    to.getZ() - from.getZ());
}

// функция строит сетку пространства треугольников
template <typename T> 
std::pair<point_t<T>, point_t<T>> compute_global_bbox(const std::vector<Triangle<T>>& triangles) 
{
    float min_x = std::numeric_limits<float>::max();
    float min_y = std::numeric_limits<float>::max();
    float min_z = std::numeric_limits<float>::max();

    float max_x = std::numeric_limits<float>::lowest();
    float max_y = std::numeric_limits<float>::lowest();
    float max_z = std::numeric_limits<float>::lowest();

    for (const auto& tri : triangles) {
        auto [local_min, local_max] = tri.get_bound_box();
        min_x = std::min(min_x, local_min.getX());
        min_y = std::min(min_y, local_min.getY());
        min_z = std::min(min_z, local_min.getZ());

        max_x = std::max(max_x, local_max.getX());
        max_y = std::max(max_y, local_max.getY());
        max_z = std::max(max_z, local_max.getZ());
    }

    return {point_t<float>(min_x, min_y, min_z),
            point_t<float>(max_x, max_y, max_z)};
}


template <typename T> 
T find_max_diam(const std::vector<Triangle<T>>& triangles) {
    T max_d = 0;

    for (const auto& tri : triangles) 
    {
        const auto& p = tri.get_points();

        T d1 = p[0].create_vector_between(p[1]).getLength();
        T d2 = p[1].create_vector_between(p[2]).getLength();
        T d3 = p[2].create_vector_between(p[0]).getLength();

        float d = std::max({d1, d2, d3});
        max_d = std::max(max_d, d);
    }

    return max_d;
}



struct CellKey {
    int x, y, z;

    bool operator==(const CellKey& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator<(const CellKey& other) const {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        return z < other.z;
    }
};

template <typename T> 
CellKey point_to_cell(const point_t<T>& p, 
                      const point_t<T>& world_min, 
                      float cell_size) 
{
    int i = static_cast<int>(std::floor((p.getX() - world_min.getX()) / cell_size));
    int j = static_cast<int>(std::floor((p.getY() - world_min.getY()) / cell_size));
    int k = static_cast<int>(std::floor((p.getZ() - world_min.getZ()) / cell_size));

    return {i, j, k};
}

template <typename T> 
std::map<CellKey, std::vector<int>> build_uniform_grid(
    const std::vector<Triangle<T>>& triangles,
    const point_t<T>& world_min,
    float cell_size)
{
    std::map<CellKey, std::vector<int>> grid;

    for (int idx = 0; idx < static_cast<int>(triangles.size()); ++idx) {
        auto [bbox_min, bbox_max] = triangles[idx].get_bound_box();

        auto min_cell = point_to_cell(bbox_min, world_min, cell_size);
        auto max_cell = point_to_cell(bbox_max, world_min, cell_size);

        auto [min_i, min_j, min_k] = min_cell;
        auto [max_i, max_j, max_k] = max_cell;


        for (int i = min_i; i <= max_i; ++i) {
            for (int j = min_j; j <= max_j; ++j) {
                for (int k = min_k; k <= max_k; ++k) {
                    grid[{i, j, k}].push_back(idx);
                }
            }
        }
    }

    return grid;
}

template <typename T> 
bool triangles_intersect(const Triangle<T>& tri_a, const Triangle<T>& tri_b);

template <typename T> 
std::vector<int> find_answer_with_grid(const std::vector<Triangle<T>>& triangles) {
    if (triangles.empty()) return {};

    auto [world_min, world_max] = compute_global_bbox(triangles);

    float max_d = find_max_diam(triangles);
    float cell_size = (max_d < EPSILON) ? 1.0f : max_d * max_d;


    auto grid = build_uniform_grid(triangles, world_min, cell_size);

    std::unordered_set<int> collided;

    // проверка внутри ячейки
    for (const auto& [cell_key, indices] : grid) {
        for (size_t i = 0; i < indices.size(); ++i) {
            for (size_t j = i + 1; j < indices.size(); ++j) {
                int idx_a = indices[i];
                int idx_b = indices[j];

                if (collided.count(idx_a) && collided.count(idx_b)) continue;

                if (triangles_intersect(triangles[idx_a], triangles[idx_b])) {
                    collided.insert(idx_a);
                    collided.insert(idx_b);
                }
            }
        }
    }

    std::vector<int> result(collided.begin(), collided.end());
    std::sort(result.begin(), result.end());

    return result;
}


template <typename T> 
bool triangles_intersect(const Triangle<T>& tri_a, const Triangle<T>& tri_b) 
{
    if (!tri_a.is_valid() || !tri_b.is_valid()) 
        return false;

    auto normal_a = tri_a.get_normal();
    auto normal_b = tri_b.get_normal();

    auto status = check_if_parallel(normal_a, normal_b, tri_b.get_points()[0], tri_a.get_points()[0]);

    if (status == IN_THE_SAME_PLANE) 
    {
        return check_the_axes_on_plane(tri_a.get_edges(), tri_b.get_edges(), tri_a, tri_b, normal_a);
    }

    if (status == IN_PARALLEL_PLANES) 
    {
        return false;
    }

    int summ_i = 0;

    point_t<float> refer_point_j = tri_b.get_points()[0];

    for (int k = 0; k < 3; k++)
    {
        if (normal_b.scalar_compos(refer_point_j.create_vector_between(tri_a.get_points()[k])) < 0)
        {   
            summ_i--;
        }

        if (normal_b.scalar_compos(refer_point_j.create_vector_between(tri_a.get_points()[k])) > 0)
        {   
            summ_i++;
        }
    }

    if (summ_i == 3 || summ_i == -3)
    {
        return false;
    }

    int summ_j = 0;

    point_t<float> refer_point_i = tri_a.get_points()[0];

    for (int k = 0; k < 3; k++)
    {
        if (normal_a.scalar_compos(refer_point_i.create_vector_between(tri_b.get_points()[k])) < 0)
        {   
            summ_j--;
        }
        
        if (normal_a.scalar_compos(refer_point_i.create_vector_between(tri_b.get_points()[k])) > 0)
        {
            summ_j++;
        }
            
    }

    std::array<vect_t<T>, 3> edges_a = tri_a.get_edges();
    std::array<vect_t<T>, 3> edges_b = tri_b.get_edges();

    if (summ_j == 3 || summ_j == -3)
    {
        return false;
    }

    else
    {
        if (check_the_edge_axes(edges_a, edges_b, tri_a, tri_b))
        {
            return true;
        }

        else
        {
            return false;
        }
    }
}