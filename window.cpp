#include <Windows.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

#include <thread>
#include <chrono>
using namespace std::literals;
#include <system_error>

#include "window.h"
#include "print.h"



namespace
{
HWND                theWindow{};
constexpr int       WM_REFRESH  {WM_APP};
constexpr auto      windowStyle { WS_OVERLAPPEDWINDOW | WS_VISIBLE    };



void paint(HWND h,WPARAM w, LPARAM l)
{
    PAINTSTRUCT paint{};
    BeginPaint(h,&paint);

    RECT        client{};
    GetClientRect(h,&client);

    ::paint(paint.hdc, client);    
    
    EndPaint(h,&paint);
}

void click(HWND h, LPARAM l)
{
    double const x = LOWORD(l);
    double const y = HIWORD(l);

    RECT  r{};
    GetClientRect(h,&r);

    addPoint( { x/(r.right-r.left),   y/(r.bottom-r.top) } );    

    PostMessage(h,WM_REFRESH,0,0);
}


LRESULT CALLBACK proc(HWND h, UINT m, WPARAM w, LPARAM l)
{
    switch(m)
    {
    case WM_CREATE:
    {        
        RECT    client{0,0, 800, 800};

        AdjustWindowRect(&client,windowStyle,FALSE);
        SetWindowPos(h, nullptr,0,0, client.right-client.left, client.bottom-client.top, SWP_NOMOVE|SWP_NOZORDER);
        SetForegroundWindow(h);
        return 0;
    }

    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        paint(h,w,l);
        return 0;

    case WM_LBUTTONDOWN:
        click(h,l);
        return 0;


    case WM_REFRESH:
        InvalidateRect(h,nullptr,TRUE);
        return 0;
    
    case WM_NCHITTEST:
    case WM_MOUSEMOVE:
    case WM_NCMOUSEMOVE:
    case WM_SETCURSOR:
        break;

    default:
        //print("msg {:#x}\n",m);
        break;
    }

    return DefWindowProc(h,m,w,l);
}


}


void createWindow()
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput{};
    ULONG_PTR                    gdiplusToken{};

    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
   
   
    WNDCLASSA    Class
    {
        CS_OWNDC | CS_VREDRAW | CS_HREDRAW,
        proc,
        0,
        0,
        GetModuleHandle(nullptr),
        nullptr,
        LoadCursor(nullptr,IDC_ARROW),
        static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)),
        nullptr,
        "DelaunayClass"
    };

    if(!RegisterClassA(&Class))
    {
        throw std::system_error{ static_cast<int>(GetLastError()), std::system_category(), "RegisterClass"};
    }

    theWindow = CreateWindowA(Class.lpszClassName,
                              "Delaunay",
                              windowStyle,
                              CW_USEDEFAULT,CW_USEDEFAULT,
                              10,10,
                              nullptr,
                              nullptr,
                              GetModuleHandle(nullptr),
                              nullptr);

    if(theWindow==nullptr)
    {
        throw std::system_error{ static_cast<int>(GetLastError()), std::system_category(), "CreateWindow"};
    }

}



void windowMessageLoop()
{
    MSG     msg;
    while(GetMessage(&msg,0,0,0) > 0)
    {
        DispatchMessage(&msg);
    }
}


void redraw()
{
    PostMessage(theWindow,WM_REFRESH,0,0);
//    Sleep(100);

}

