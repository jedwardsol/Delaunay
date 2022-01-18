#include <Windows.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

#include <vector>
#include <stdexcept>
#include <algorithm>

#include "window.h"
#include "geometry.h"

std::vector<Point>    points;
std::vector<Triangle> triangles;






void insertTriangle(Point const &a,Point const &b,Point const &c)
{
    Vector const ab = b-a;
    Vector const ac = c-a;

    auto  x = crossProduct(ab,ac);

    if(x < 0)
    {
        triangles.emplace_back(a,b,c);
    }
    else
    {
        triangles.emplace_back(a,c,b);
    }
}



Triangle    findTriangle(Point const &point)
{
    for(auto const triangle : triangles)
    {
        if(point.inside(triangle))
        {
            return triangle;
        }
    }

    throw std::runtime_error("containing triangle not found");
}


void addPoint(Point const &point)
{
    points.push_back(point);


    auto const container = findTriangle(point);


    insertTriangle(container.a,container.b, point);
    insertTriangle(container.b,container.c, point);
    insertTriangle(container.c,container.a, point);

    auto erase = std::ranges::remove(triangles,container);
    triangles.erase(erase.begin(),erase.end());



}

void makeSuperTriangle()
{
#if defined _DEBUG
    Point  a{ 0.01,  0.99 };
    Point  b{ 0.99,  0.99 };
    Point  c{ 0.5,   0.01 };
#else
    Point  a{ -90,  90 };
    Point  b{  90,  90 };
    Point  c{ 0.5, -90 };
#endif

    insertTriangle(a,b,c);
}




int main()
{
    makeSuperTriangle();

    createWindow();
    windowMessageLoop();
}