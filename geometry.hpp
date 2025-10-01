#pragma once

#include <unordered_map>
#include <vector>

#include <math.h>

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

    float project_point(const vect_t<float> axis) const
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


// check if coord are valid
bool is_valid_triangle(const std::vector<point_t<float>>& pts);

// check if tringles in parallel planes
enum Parallel_Or_Not check_if_parallel(const vect_t<float>& normal_vec_first, const vect_t<float>& normal_vec_second, const point_t<float>& point_in_second, const point_t<float>& point_in_first);


bool check_the_edge_axes(const std::vector<vect_t<float>>& i_triangle, 
                        const std::vector<vect_t<float>>& j_triangle, 
                        const std::unordered_map<int, std::vector<point_t<float>>> &hash_of_triangles,
                        int tri_i_index, int tri_j_index);

bool check_the_axes_on_plane(const std::vector<vect_t<float>>& i_triangle, 
                        const std::vector<vect_t<float>>& j_triangle, 
                        const std::unordered_map<int, std::vector<point_t<float>>>& hash_of_triangles,
                        int tri_i_index, int tri_j_index, 
                        const vect_t<float>& normal_vect);

std::vector<int> find_answer(const std::unordered_map<int, std::vector<point_t<float>>> &hash_of_triangles, int N);

template <typename T> 
vect_t<T> create_vector_between(const point_t<T> from, const point_t<T> to) 
{
    return vect_t<T>(to.getX() - from.getX(), 
                    to.getY() - from.getY(), 
                    to.getZ() - from.getZ());
}