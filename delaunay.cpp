#include <Windows.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

#include <cassert>
#include <vector>
#include <queue>
#include <stdexcept>
#include <algorithm>
#include <thread>

#include "print.h"
#include "window.h"
#include "geometry.h"
#include "window.h"

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

    redraw();
}


void removeTriangle(Triangle const &triangle)
{
    auto [begin,end] = std::ranges::remove(triangles,triangle);

    assert( begin != end);
    
    triangles.erase(begin,end);

    redraw();
}




Triangle    findSurroundingTriangle(Point const &point)
{
    for(auto const &triangle : triangles)
    {
        if(point.inside(triangle))
        {
            return triangle;
        }
    }

    throw std::runtime_error("containing triangle not found");
}


std::vector<Triangle> findNeighbourTriangles(Line const &line)
{
    std::vector<Triangle>  neighbours;

    for(auto const &triangle : triangles)
    {
        if(triangle.contains(line))
        {
            neighbours.push_back(triangle);
        }
    }

    assert(   neighbours.size()==1
           || neighbours.size()==2);

    return neighbours;
}


bool notDelaunay(Triangle triangle, Point const &point)
{
    while(onLeft(triangle.c, triangle.a, point))
    {
        triangle.rotate();
    }

    return triangle.circleSurrounds(point);
}




/*

Candidate line b-c has 2 neighbour triangles

         b
        /|\
       / | \
      a  |  d
       \ | /
        \|/
         c

If the triangles are not Delaunay then flip

         b
        / \
       /   \
      a-----d 
       \   /
        \ /
         c


remove a-c-d  and b-c-d

add a-c-d and a-d-b

*/
void performFlips(std::queue<Line>      &candidates)
{
    while(!candidates.empty())
    {
        auto const candidate = candidates.front();
        candidates.pop();

        auto neighbours{findNeighbourTriangles(candidate)};

        if(neighbours.size()==1)
        {
            continue;
        }

        auto a = neighbours[0].thirdPoint(candidate);
        auto b = neighbours[1].thirdPoint(candidate);

        if(    notDelaunay(neighbours[0],b)
           ||  notDelaunay(neighbours[1],a))
        {
            removeTriangle(neighbours[0]);
            removeTriangle(neighbours[1]);

            insertTriangle(candidate.a, a,b);
            insertTriangle(candidate.b, a,b);

            candidates.emplace(candidate.a,a);
            candidates.emplace(candidate.a,b);
            
            candidates.emplace(candidate.b,a);
            candidates.emplace(candidate.b,b);
        }
    }
}



void addPointThread(Point const &point)
{
    points.push_back(point);


    auto const container = findSurroundingTriangle(point);

    insertTriangle(container.a,container.b, point);
    insertTriangle(container.b,container.c, point);
    insertTriangle(container.c,container.a, point);


    std::queue<Line>      flipCandidates;

    flipCandidates.emplace(container.a,container.b);
    flipCandidates.emplace(container.b,container.c);
    flipCandidates.emplace(container.c,container.a);

    removeTriangle(container);

    performFlips(flipCandidates);
}

void addPoint(Point const &point)
{
    std::thread{addPointThread,point}.detach();
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