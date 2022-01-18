#pragma once

#include <Windows.h>


struct Point
{
    double x;
    double y;
};


void createWindow();
void windowMessageLoop();

void paint(HDC, RECT const &);
void click(Point const &);