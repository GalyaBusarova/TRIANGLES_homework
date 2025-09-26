#include "geometry.hpp"

template <typename T> 
vect_t<T> create_vector_between(const point_t<T> from, const point_t<T> to) 
{
    return vect_t<T>(to.getX() - from.getX(), 
                    to.getY() - from.getY(), 
                    to.getZ() - from.getZ());
}
