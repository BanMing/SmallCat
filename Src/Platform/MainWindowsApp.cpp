#include "MainApp.h"

#if BX_PLATFORM_WINDOWS

#include <windows.h>

#include <bx/mutex.h>
#include <bx/handlealloc.h>

#include "../Events/EventQueue.h"
#include "../Contanst/AppConst.h"
namespace Tiga
{
#pragma region Window Context
    struct Context
    {
        EventQueue mEventQueue;
        WCHAR mSurrogate;
        bx::Mutex mLock;

        bx::HandleAllocT<kMaxWindows> mWindowAlloc;
        HWND mHWND[kMaxWindows];
        uint32_t mFlags[kMaxWindows];

        RECT mRect;
        DWORD mStlye;
        uint32_t mWidth;
        uint32_t mHeight;
        uint32_t mOldWidth;
        uint32_t mOldHeight;
        uint32_t mFrameWidth;
        uint32_t mFrameHeight;
        float mAspectRatio;

        int32_t mMouseX;
        int32_t mMouseY;
        int32_t mMouseZ;

        bool mFrame;
        HWND mMouseLock;
        bool mInit;
        bool mExit;

        static LRESULT CALLBACK WndProc(HWND _hwnd, UINT _id, WPARAM _wparam, LPARAM _lparam);

        Context() : mMouseZ(0), mFrame(true), mMouseLock(NULL), mInit(false), mExit(false)
        {
        }

        int32_t Run(int argc, const char *const *argv)
        {
            SetDllDirectoryA(".");

            HINSTANCE instance = (HINSTANCE)GetModuleHandle(NULL);

            WNDCLASSEXW wnd;
            bx::memSet(&wnd, 0, sizeof(wnd));
            wnd.cbSize = sizeof(wnd);
            wnd.style = CS_HREDRAW | CS_VREDRAW;
            wnd.lpfnWndProc = WndProc;
            wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
            wnd.lpszClassName = L"Tiga";
            wnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
            RegisterClassExW(&wnd);

            mWindowAlloc.alloc();
            mHWND[kMainWindowIndex] = CreateWindowExA(WS_EX_ACCEPTFILES, "Tiga", "Tiga",
                                                      WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0,
                                                      kAppWidth, kAppHeight, NULL, NULL, instance, 0);

            mFlags[kMainWindowIndex] = 0 | WINDOW_FLAG_ASPECT_RATIO | WINDOW_FLAG_FRAME;

            // sets hwnd to platform data
            bgfx::PlatformData pd;
            bx::memSet(&pd, 0, sizeof(pd));
            pd.nwh = mHWND[kMainWindowIndex];
            bgfx::setPlatformData(pd);

            Adjust(mHWND[kMainWindowIndex], kAppWidth, kAppHeight, true);
            Clear(mHWND[kMainWindowIndex]);

            mWidth = kAppWidth;
            mHeight = kAppHeight;
            mOldWidth = kAppWidth;
            mOldHeight = kAppHeight;
        }

        WindowHandle FindHandle(HWND hwnd)
        {
            bx::MutexScope scope(mLock);
            for (uint16_t i = 0, num = mWindowAlloc.getMaxHandles(); i < num; i++)
            {
                uint16_t idx = mWindowAlloc.getHandleAt(i);
                if (hwnd == mHWND[idx])
                {
                    WindowHandle handle = {idx};
                    return handle;
                }
                
                WindowHandle invalid = {UINT16_MAX};
                return invalid;
            }
        }

        void Clear(HWND hwnd)
        {
            RECT rect;
            GetWindowRect(hwnd, &rect);
            HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
            HDC hdc = GetDC(hwnd);
            SelectObject(hdc, brush);
            FillRect(hdc, &rect, brush);
            ReleaseDC(hwnd, hdc);
            DeleteObject(brush);
        }

        void Adjust(HWND hwnd, uint32_t width, uint32_t height, bool windowFrame)
        {
            mWidth = width;
            mHeight = height;
            mAspectRatio = float(width) / float(height);

            ShowWindow(hwnd, SW_SHOWNORMAL);
            RECT rect;
            RECT newrect = {0, 0, (LONG)width, (LONG)height};
            DWORD style = WS_POPUP | WS_SYSMENU;

            if (mFrame)
            {
                GetWindowRect(hwnd, &mRect);
                mStlye = GetWindowLong(hwnd, GWL_STYLE);
            }

            if (windowFrame)
            {
                rect = mRect;
                style = mStlye;
            }
            else
            {
                HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
                MONITORINFO mi;
                mi.cbSize = sizeof(mi);
                GetMonitorInfo(monitor, &mi);
                newrect = mi.rcMonitor;
                rect = mi.rcMonitor;
                mAspectRatio = float(newrect.right - newrect.left) / float(newrect.bottom - newrect.top);
            }

            SetWindowLong(hwnd, GWL_STYLE, style);
            uint32_t preWidth = newrect.right - newrect.left;
            uint32_t preHeight = newrect.bottom - newrect.top;
            AdjustWindowRect(&newrect, style, FALSE);
            mFrameWidth = (newrect.right - newrect.left) - preWidth;
            mFrameHeight = (newrect.bottom - newrect.top) - preHeight;
            UpdateWindow(hwnd);

            if (rect.left == -32000 || rect.top == -32000)
            {
                rect.left = 0;
                rect.top = 0;
            }

            int32_t left = rect.left;
            int32_t top = rect.top;
            int32_t width = newrect.right - newrect.left;
            int32_t height = newrect.bottom - newrect.top;

            if (!windowFrame)
            {
                float aspectRatio = 1.0f / mAspectRatio;
                width = bx::uint32_max(kAppWidth / 4, width);
                height = uint32_t(float(width) * aspectRatio);

                left = newrect.left + (newrect.right - newrect.left - width) / 2;
                top = newrect.top + (newrect.bottom - newrect.top - height) / 2;
            }

            SetWindowPos(hwnd, HWND_TOP, left, top, width, height, SWP_SHOWWINDOW);

            mFrame = windowFrame;
        }

        void SetMousePos(HWND hwnd, int32_t mouseX, int32_t mouseY)
        {
            POINT pt = {mouseX, mouseY};
            ClientToScreen(hwnd, &pt);
            SetCursorPos(pt.x, pt.y);
        }

        void SetMouseLock(HWND hwnd, bool lock)
        {
            HWND newMouseLock = lock ? hwnd : 0;

            if (newMouseLock != mMouseLock)
            {
                if (lock)
                {
                    mMouseX = mWidth / 2;
                    mMouseY = mHeight / 2;
                    ShowCursor(false);
                    SetMousePos(hwnd, mMouseX, mMouseY);
                }
                else
                {
                    SetMousePos(hwnd, mMouseX, mMouseY);
                    ShowCursor(true);
                }
                mMouseLock = newMouseLock;
            }
        }
    };

#pragma endregion Window Context

#pragma region WindowHandle
#pragma endregion WindowHandle

    int32_t Run(Application *app, int argc, const char *const *argv)
    {
    }
} // namespace Tiga

#endif