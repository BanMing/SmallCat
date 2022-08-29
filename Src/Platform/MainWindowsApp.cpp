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

        void Adjust(HWND hwnd, uint32_t width, uint32_t height, bool widowFrame) {
            mWidth=width;
            mHeight=height;
            mAspectRatio=float(width)/float(height);
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