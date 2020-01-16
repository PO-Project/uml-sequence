#ifndef _UTILITY_HPP
#define _UTILITY_HPP

#include <vector>
#include <algorithm>

namespace Utility{

template <typename T>
struct PointT{
        T x, y;

        PointT() = default;
        PointT(T x, T y) : x(x), y(y) {}
        PointT(PointT &) = default;
        PointT(PointT &&) = default;

        PointT operator-(const PointT<T> &v)
        {
            return PointT(this->x - v.x, this->y - v.y);
        }
        PointT operator+(const PointT<T> &v)
        {
            return PointT(this->x + v.x, this->y + v.y);
        }

        PointT operator-(T v)
        {
            return PointT(this->x - v, this->y - v);
        }
        PointT operator+(T v)
        {
            return PointT(this->x + v, this->y + v);
        }

        PointT operator/(T v)
        {
            return PointT(this->x / v, this->y / v);
        }
        void operator=(const PointT<T> &p)
        {
            this->x = p.x;
            this->y = p.y;
        }

};

using Point = PointT<int>;

template <typename T>
auto binaryInsert(std::vector<T> & v, T const & val)
{
    return v.insert(std::upper_bound(v.begin(), v.end(), val), val);
}


}

#endif