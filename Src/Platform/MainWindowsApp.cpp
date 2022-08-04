#include <bgfx/platform.h>
// #if BX_PLATFORM_EMSCRIPTEN
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
#include "../Imgui/DearImgui.h"
namespace Tiga
{
    Application *gApplication;
    LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

    int32_t Run(Application *app, int argc, const char *const *argv)
    {
        gApplication = app;

#pragma region Windows Class
        HINSTANCE instance = (HINSTANCE)GetModuleHandle(NULL);

        WNDCLASSEX wndclass;
        wndclass.cbSize = sizeof(WNDCLASSEX);
        wndclass.style = CS_HREDRAW | CS_VREDRAW;
        wndclass.lpfnWndProc = WndProc;
        wndclass.cbClsExtra = 0;
        wndclass.cbWndExtra = 0;
        wndclass.hInstance = instance;
        wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
        wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
        wndclass.lpszMenuName = 0;
        wndclass.lpszClassName = "Tiga";
        RegisterClassEx(&wndclass);

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
        AdjustWindowRectEx(&windowRect, style, FALSE, 0);
        HWND hwnd = CreateWindowEx(WS_EX_ACCEPTFILES, "Tiga", "Tiga", style,
                                   windowRect.left, windowRect.top,
                                   windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
                                   NULL, NULL, instance, 0);

        HDC hdc = GetDC(hwnd);

        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 24;
        pfd.cDepthBits = 32;
        pfd.cStencilBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;
        int pixelFormat = ChoosePixelFormat(hdc, &pfd);
        SetPixelFormat(hdc, pixelFormat, &pfd);

        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
#pragma endregion Windows Class

#pragma region bgfx init
        // set platform window data
        bgfx::PlatformData pd;
        bx::memSet(&pd, 0, sizeof(pd));
        pd.nwh = hwnd;
        bgfx::setPlatformData(pd);

        bgfx::renderFrame();

        bgfx::Init init;
        init.type = bgfx::RendererType::Direct3D12;
        init.vendorId = BGFX_PCI_ID_NONE;
        init.resolution.width = clientWidth;
        init.resolution.height = clientHeight;
        init.resolution.reset = BGFX_RESET_VSYNC;
        bgfx::init(init);

        // Set view 0 clear state.
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
#pragma endregion bgfx init
        CreateGUI();
        app->Initialize();

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
            float aspect = (float)clientWidth / (float)clientHeight;
            app->Render(aspect);
#pragma endregion Render

#pragma region IMGUI Update
            app->OnGUI();
#pragma endregion IMGUI Update
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
                ShutdownGUI();
                // Shutdown bgfx.
                bgfx::shutdown();
                gApplication = 0;
                DestroyWindow(hwnd);
            }
            else
            {
                std::cout << "Already shut down!\n";
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        }

        return DefWindowProc(hwnd, iMsg, wParam, lParam);
    }
} // namespace Tiga

#endif