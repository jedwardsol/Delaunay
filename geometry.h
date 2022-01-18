#pragma once

struct Point            // 0 to 1
{
    double x;
    double y;
};

struct Vector
{
    double dx;
    double dy;
};

struct Triangle
{
    Point a;
    Point b;
    Point c;
};


inline Vector operator-(Point const &lhs, Point const &rhs) noexcept
{
    return {lhs.x-rhs.x, lhs.y-rhs.y};
}


inline Vector operator/(Vector const &lhs, double scale) noexcept
{
    return {lhs.dx/scale, lhs.dy/scale};
}

inline Vector operator*(Vector const &lhs, double scale) noexcept
{
    return {lhs.dx*scale, lhs.dy*scale};
}


inline Point operator+(Point const &lhs, Vector const &rhs) noexcept
{
    return {lhs.x+rhs.dx, lhs.y+rhs.dy};
}



inline double crossProduct(Vector const &lhs, Vector const &rhs) noexcept
{
    return lhs.dx * rhs.dy  - lhs.dy * rhs.dx;
}