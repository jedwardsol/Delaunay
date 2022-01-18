#include "window.h"

#include <vector>

std::vector<Point> points;


void paint(HDC dc, RECT const &r)
{
    
    for(auto const point : points)
    {
        SetPixel(dc,
                 point.x * (r.right-r.left),
                 point.y * (r.bottom-r.top),
                 RGB(255,255,255));

    }


}


void click(Point const &p)
{
    points.push_back(p);
}


int main()
{
    createWindow();
    windowMessageLoop();
}