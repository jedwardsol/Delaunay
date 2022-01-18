#include <Windows.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")


#include <vector>

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

    pen.SetWidth(4);
    pen.SetEndCap(Gdiplus::LineCapArrowAnchor);
    graphics.DrawLine(&pen,  a,middle);

}

void paint(HDC dc, RECT const &client)
{
    constexpr auto              circleSize{4};
                                                       //  A    R    G    B
    static Gdiplus::SolidBrush  redBrush  {Gdiplus::Color(255, 255,   0,   0)};
    static Gdiplus::SolidBrush  greenBrush{Gdiplus::Color(255,   0, 255,   0)};

    static Gdiplus::Pen         whitePen{Gdiplus::Color(255, 255, 255, 255)};
    static Gdiplus::Pen         redPen  {Gdiplus::Color(255, 255,   0,   0)};
    static Gdiplus::Pen         greenPen{Gdiplus::Color(255,   0, 255,   0)};
    static Gdiplus::Pen         bluePen {Gdiplus::Color(255,   0,   0, 255)};


    Gdiplus::Graphics graphics{dc};

    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

    if(triangles.empty())
    {
        for(auto const point : points)
        {
            auto p = toClient(client,point);

            graphics.FillEllipse(&redBrush, p.X-circleSize/2,p.Y-circleSize/2,circleSize,circleSize);
            graphics.DrawEllipse(&whitePen, p.X-circleSize/2,p.Y-circleSize/2,circleSize,circleSize);
        }
    }
    else
    {
        for(auto const point : points)
        {
            auto p = toClient(client,point);

            if(inside(triangles.front(), point))
            {
                graphics.FillEllipse(&greenBrush, p.X-circleSize/2,p.Y-circleSize/2,circleSize,circleSize);
            }
            else
            {
                graphics.FillEllipse(&redBrush, p.X-circleSize/2,p.Y-circleSize/2,circleSize,circleSize);
            }
        }
    }


    for(auto const triangle : triangles)
    {
        arrowLine(graphics,redPen,  client,triangle.a,triangle.b);
        arrowLine(graphics,greenPen,client,triangle.b,triangle.c);
        arrowLine(graphics,bluePen, client,triangle.c,triangle.a);
    }
}




void click(Point const &p)
{
    points.push_back(p);

    if( triangles.empty())
    {
        if(points.size()==3)
        {
            Vector  v01 = points[1]-points[0];
            Vector  v02 = points[2]-points[0];

            auto  x = crossProduct(v01,v02);

            if(x < 0)
            {
                triangles.emplace_back(points[0],points[1],points[2]);
            }
            else
            {
                triangles.emplace_back(points[0],points[2],points[1]);
            }

            points.clear();
        }
    }
    else
    {

    }

}


int main()
{
    createWindow();
    windowMessageLoop();
}