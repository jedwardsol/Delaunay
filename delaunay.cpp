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




Gdiplus::Point toClient(RECT const &client, Point const &point)
{
    auto x = static_cast<int>(point.x * (client.right -client.left));
    auto y = static_cast<int>(point.y * (client.bottom-client.top));

    return {x,y};
}


void arrowLine(Gdiplus::Graphics    &graphics,
               Gdiplus::Pen         &pen,
               RECT const           &client,
               Point const          &start,
               Point const          &end)
{
    auto a      = toClient(client,start);
    auto b      = toClient(client,end);
    auto middle = toClient(client,start+(end-start)/5);

    pen.SetWidth(1);
    pen.SetEndCap(Gdiplus::LineCapNoAnchor);
    graphics.DrawLine(&pen,  a,b);

    pen.SetWidth(2);
    pen.SetEndCap(Gdiplus::LineCapArrowAnchor);
    graphics.DrawLine(&pen,  a,middle);
}

void line(Gdiplus::Graphics    &graphics,
               Gdiplus::Pen         &pen,
               RECT const           &client,
               Point const          &start,
               Point const          &end)
{
    auto a      = toClient(client,start);
    auto b      = toClient(client,end);

    graphics.DrawLine(&pen,  a,b);
}




void paint(HDC dc, RECT const &client)
{
    constexpr auto              circleSize{4};
                                                       
    static Gdiplus::SolidBrush  whiteBrush  {Gdiplus::Color::White};
    static Gdiplus::SolidBrush  redBrush    {Gdiplus::Color::Red};
    static Gdiplus::SolidBrush  greenBrush  {Gdiplus::Color::Green};
    static Gdiplus::Pen         whitePen    {Gdiplus::Color::White};
    static Gdiplus::Pen         redPen      {Gdiplus::Color::Red};
    static Gdiplus::Pen         greenPen    {Gdiplus::Color::Green};
    static Gdiplus::Pen         bluePen     {Gdiplus::Color::Blue};


    Gdiplus::Graphics graphics{dc};

    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);



    for(auto const &triangle : triangles)
    {
        line(graphics,whitePen, client,triangle.a,triangle.b);
        line(graphics,whitePen, client,triangle.b,triangle.c);
        line(graphics,whitePen, client,triangle.c,triangle.a);
    }

    for(auto const &point : points)
    {
        auto p = toClient(client,point);

        graphics.FillEllipse(&whiteBrush, p.X-circleSize/2,p.Y-circleSize/2,circleSize,circleSize);
    }
}



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



void click(Point const &point)
{
    points.push_back(point);


    for(auto const triangle : triangles)
    {
        if(point.inside(triangle))
        {
            insertTriangle(triangle.a,triangle.b, point);
            insertTriangle(triangle.b,triangle.c, point);
            insertTriangle(triangle.c,triangle.a, point);

            auto erase = std::ranges::remove(triangles,triangle);

            triangles.erase(erase.begin(),erase.end());

            return;
        }
    }

    throw std::runtime_error("containing triangle not found");
}

void makeSuperTriangle()
{
/*
    Point  a{ 0.1,  0.9 };
    Point  b{ 0.9,  0.9 };
    Point  c{ 0.5,  0.1 };
*/

    Point  a{ -90,  90 };
    Point  b{  90,  90 };
    Point  c{ 0.5, -90 };

    insertTriangle(a,b,c);
}



int main()
{
    makeSuperTriangle();
        

    createWindow();
    windowMessageLoop();
}