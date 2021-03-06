#pragma once

#include <Windows.h>

#include "geometry.h"


void createWindow();
void windowMessageLoop();

void paint(HDC, RECT const &);
void addPoint(Point const &);
void redraw();
