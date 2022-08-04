#include <bgfx/platform.h>

#if BX_PLATFORM_WINDOWS
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
    LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

    int32_t Run(Application *app, int argc, const char *const *argv)
    {
        HINSTANCE instance = (HINSTANCE)GetModuleHandle(NULL);

        WNDCLASSEXW wnd;
        wnd.cbSize = sizeof(wnd);
        wnd.style = CS_HREDRAW | CS_VREDRAW;
        wnd.lpfnWndProc = WndProc;
        wnd.hInstance = instance;
        wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
        wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
        wnd.lpszClassName = L"Tiga";
        RegisterClassExW(&wnd);

        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        int clientWidth = 800;
        int clientHeight = 600;
        RECT windowRect;
        SetRect(&windowRect, (screenWidth / 2) - (clientWidth / 2),
                (screenHeight / 2) - (clientHeight / 2),
                (screenWidth / 2) + (clientWidth / 2),
                (screenHeight / 2) + (clientHeight / 2));

        // WS_THICKFRAME to resize
        DWORD style = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
        HWND hwnd = CreateWindowExA(WS_EX_ACCEPTFILES, "Tiga", "Tiga", style,
                                    windowRect.left, windowRect.top,
                                    windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
                                    NULL, NULL, instance, 0);

        // set platform window data
        bgfx::PlatformData pd;
        bx::memSet(&pd, 0, sizeof(pd));
        pd.nwh = hwnd;
        bgfx::setPlatformData(pd);

        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
        app->Initialize();

        bgfx::renderFrame();

        MSG msg;
        DWORD lastTick = GetTickCount();
        while (true)
        {
            bgfx::renderFrame();
#pragma region Events
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    break;
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
#pragma endregion Events

#pragma region Update
            DWORD thisTick = GetTickCount();
            float deltaTime = float(thisTick - lastTick) * 0.001f;
            lastTick = thisTick;
            app->Update(deltaTime);
#pragma endregion Update

#pragma region Render
            app->Render(deltaTime);

            // Set view 0 default viewport.
            // bgfx::setViewRect(0, 0, 0, clientWidth, clientHeight);
            // This dummy draw call is here to make sure that view 0 is cleared
            // if no other draw calls are submitted to view 0.
            // bgfx::touch(0);
            // Advance to next frame. Rendering thread will be kicked to
            // process submitted rendering primitives.
            bgfx::frame();
#pragma endregion Render

#pragma region IMGUI Update
            app->OnGUI();
#pragma endregion IMGUI Update
        }

        return (int)msg.wParam;
    }

    LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
    {
        return DefWindowProc(hwnd, iMsg, wParam, lParam);
    }
} // namespace Tiga

#endif