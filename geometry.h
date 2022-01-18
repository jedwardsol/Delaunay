#pragma once

#include <array>

using threeXthree = std::array<std::array<double,3>,3>;


/*

  | a b c |
  | d e f |  = aei + bfg + cdh - ceg - bdi - afh
  | g h i |

*/

inline auto determinant(threeXthree const &matrix)
{
    auto a = matrix[0][0];
    auto b = matrix[0][1];
    auto c = matrix[0][2];

    auto d = matrix[1][0];
    auto e = matrix[1][1];
    auto f = matrix[1][2];

    auto g = matrix[2][0];
    auto h = matrix[2][1];
    auto i = matrix[2][2];


    auto aei = a * e * i;
    auto bfg = b * f * g;
    auto cdh = c * d * h;
    auto ceg = c * e * g;
    auto bdi = b * d * i;
    auto afh = a * f * h;

    return aei + bfg + cdh - ceg - bdi - afh;
}




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

struct Line
{
    Point a;
    Point b;

    bool operator==(Line const &) const noexcept = default;
};

struct Triangle
{
    Point a;
    Point b;
    Point c;

    bool operator==(Triangle const &) const noexcept = default;


    bool contains(Point const &point) const noexcept
    {
        return    point == a
               || point == b
               || point == c;
    }

    bool contains(Line const &line) const noexcept
    {
        return    contains(line.a)
               && contains(line.b);
    }

    Point thirdPoint(Line const &line) const noexcept
    {
        if(    a!= line.a
           &&  a!= line.b)
        {
            return a;
        }
        else if(    b!= line.a
                &&  b!= line.b)
        {
            return b;
        }
        else
        {
            return c;
        }
    }

    void rotate()
    {
        // abc -> bca
        std::swap(a,b);  
        std::swap(c,b);  
    }




    // a - b - c - point   must be anti-clockwise
    // d has to be the right of c-a
    bool circleSurrounds(Point const &point) const noexcept
    {
        auto term = [&](Point const &vertex)
        {
            return  ((vertex.x * vertex.x) - (point.x*point.x))
                  + ((vertex.y * vertex.y) - (point.y*point.y));
        };


        threeXthree  matrix
        {{
            {a.x-point.x, a.y-point.y, term(a)},
            {b.x-point.x, b.y-point.y, term(b)},
            {c.x-point.x, c.y-point.y, term(c)},
        }};

        return determinant(matrix) < 0;
    }


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
