#include <bgfx/platform.h>
#if BX_PLATFORM_EMSCRIPTEN

// #if BX_PLATFORM_WINDOWS

#include <bx/mutex.h>
#include <bx/handlealloc.h>
#include <bx/os.h>
#include <bx/thread.h>
#include <bx/timer.h>
#include <bx/uint32_t.h>
#include <iostream>

#include <Windows.h>

#include "MainApp.h"
namespace Tiga
{
    Application *gApplication;
    LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

    int32_t Run(Application *app, int argc, const char *const *argv)
    {

        HINSTANCE hInstance = GetModuleHandle(NULL);
        PSTR szCmdLine = GetCommandLineA();
        WNDCLASSEX wndclass;
        wndclass.cbSize = sizeof(WNDCLASSEX);
        wndclass.style = CS_HREDRAW | CS_VREDRAW;
        wndclass.lpfnWndProc = WndProc;
        wndclass.cbClsExtra = 0;
        wndclass.cbWndExtra = 0;
        wndclass.hInstance = hInstance;
        wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
        wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
        wndclass.lpszMenuName = 0;
        wndclass.lpszClassName = "Win32 Game Window";
        RegisterClassEx(&wndclass);

        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        int clientWidth = 800;
        int clientHeight = 600;
        RECT windowRect;
        SetRect(&windowRect, (screenWidth / 2) - (clientWidth / 2), (screenHeight / 2) - (clientHeight / 2), (screenWidth / 2) + (clientWidth / 2), (screenHeight / 2) + (clientHeight / 2));

        DWORD style = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX); // WS_THICKFRAME to resize
        AdjustWindowRectEx(&windowRect, style, FALSE, 0);
        HWND hwnd = CreateWindowEx(0, wndclass.lpszClassName, "Game Window", style, windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInstance, szCmdLine);
        HDC hdc = GetDC(hwnd);

        // PIXELFORMATDESCRIPTOR pfd;
        // memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
        // pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        // pfd.nVersion = 1;
        // pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
        // pfd.iPixelType = PFD_TYPE_RGBA;
        // pfd.cColorBits = 24;
        // pfd.cDepthBits = 32;
        // pfd.cStencilBits = 8;
        // pfd.iLayerType = PFD_MAIN_PLANE;
        // int pixelFormat = ChoosePixelFormat(hdc, &pfd);
        // SetPixelFormat(hdc, pixelFormat, &pfd);

        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);

        DWORD lastTick = GetTickCount();
        MSG msg;
        while (true)
        {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    break;
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        if (gApplication != 0)
        {
            std::cout << "Expected application to be null on exit\n";
            delete gApplication;
        }

        return (int)msg.wParam;
    }

    LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (iMsg)
        {
        case WM_CLOSE:
            if (gApplication != 0)
            {
                gApplication->Shutdown();
                gApplication = 0;
                DestroyWindow(hwnd);
            }
            else
            {
                std::cout << "Already shut down!\n";
            }
            break;
        }

        return DefWindowProc(hwnd, iMsg, wParam, lParam);
    }
} // namespace Tiga

#endif