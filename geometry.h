#pragma once


struct Triangle;

struct Point            // 0 to 1
{
    double x;
    double y;

    bool inside(Triangle const &) const noexcept;

    bool operator==(Point const &) const noexcept = default;

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

    bool operator==(Triangle const &) const noexcept = default;
};

// Vector from 2 points
inline Vector operator-(Point const &lhs, Point const &rhs) noexcept
{
    return {lhs.x-rhs.x, lhs.y-rhs.y};
}

// Scale vector
inline Vector operator/(Vector const &lhs, double scale) noexcept
{
    return {lhs.dx/scale, lhs.dy/scale};
}

// Scale vector
inline Vector operator*(Vector const &lhs, double scale) noexcept
{
    return {lhs.dx*scale, lhs.dy*scale};
}


// Point from tranlating another point
inline Point operator+(Point const &lhs, Vector const &rhs) noexcept
{
    return {lhs.x+rhs.dx, lhs.y+rhs.dy};
}

inline double crossProduct(Vector const &lhs, Vector const &rhs) noexcept
{
    return lhs.dx * rhs.dy  - lhs.dy * rhs.dx;
}

inline bool onLeft(Point const &start,  Point const &end, Point const &p) noexcept
{
    auto const v1=end-start;
    auto const v2=p-start;

    return crossProduct(v2,v1) > 0;
}


inline bool Point::inside(Triangle const &triangle) const noexcept
{
    return (    onLeft(triangle.a,triangle.b, *this)
            &&  onLeft(triangle.b,triangle.c, *this)
            &&  onLeft(triangle.c,triangle.a, *this));
}
