#include "MainApp.h"

#if BX_PLATFORM_WINDOWS

#include <windows.h>
#include <windowsx.h>
#include <string>
#include <vector>

#include <bx/mutex.h>
#include <bx/handlealloc.h>

#include "../Events/Event.h"
#include "../Events/EventQueue.h"
#include "../Events/SizeEvent.h"
#include "../Events/MouseEvent.h"
#include "../Events/WindowEvent.h"
#include "../Events/CharEvent.h"
#include "../Events/DropFileEvent.h"
#include "../Contanst/AppConst.h"
#include "../Imgui/DearImgui.h"

namespace Tiga
{
#pragma region User Window Events
    enum
    {
        WM_USER_WINDOW_CREATE = WM_USER,
        WM_USER_WINDOW_DESTROY,
        WM_USER_WINDOW_SET_TITLE,
        WM_USER_WINDOW_SET_FLAGS,
        WM_USER_WINDOW_SET_POS,
        WM_USER_WINDOW_SET_SIZE,
        WM_USER_WINDOW_TOGGLE_FRAME,
        WM_USER_WINDOW_MOUSE_LOCK,
    };
#pragma endregion User Window Events

#pragma region User Window Event Message
    struct Msg
    {
        Msg() : mX(0), mY(0), mWidth(0), mHeight(0), mFlags(0), mFlagsEnabled(false)
        {
        }

        int32_t mX;
        int32_t mY;
        uint32_t mWidth;
        uint32_t mHeight;
        uint32_t mFlags;
        std::string mTitle;
        bool mFlagsEnabled;
    };
#pragma endregion User Window Event Message

#pragma region Helpers
    inline std::vector<WCHAR> UTF8ToUTF16(const char *utf8_str)
    {
        int len = MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, NULL, 0);
        std::vector<WCHAR> utf16(len);
        MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, utf16.data(), len);
        return utf16;
    }

    static void MouseCapture(HWND hwnd, bool capture)
    {
        if (capture)
        {
            SetCapture(hwnd);
        }
        else
        {
            ReleaseCapture();
        }
    }
#pragma endregion Helpers

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

        static LRESULT CALLBACK WndProc(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam);
        LRESULT CALLBACK ProcessEvent(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam);

        Context() : mMouseZ(0), mFrame(true), mMouseLock(NULL), mInit(false), mExit(false)
        {
        }

        int32_t Run(Application *app, int argc, const char *const *argv)
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

#pragma region bgfx initialize
            bgfx::renderFrame();
            bgfx::Init init;
            init.type = bgfx::RendererType::Direct3D12;
            init.vendorId = 0;
            init.resolution.width = mWidth;
            init.resolution.height = mHeight;
            init.resolution.reset = BGFX_RESET_VSYNC;
            bgfx::init(init);

            // Set view 0 clear state.
            bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

            // // Enable debug text.
            bgfx::setDebug(BGFX_DEBUG_TEXT);
#pragma endregion bgfx initialize
            // app init
            app->Initialize();

            imguiCreate();

            mInit = true;
            mEventQueue.Post(SizeEvent::GetEvent(FindHandle(mHWND[kMainWindowIndex]), mWidth, mHeight));

            MSG msg;
            msg.message = WM_NULL;

            DWORD lastTick = GetTickCount();

            while (!mExit)
            {
                bgfx::renderFrame();

                WaitForInputIdle(GetCurrentProcess(), 16);

                while (PeekMessageW(&msg, NULL, 0U, 0U, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessageW(&msg);
                }

                DWORD thisTick = GetTickCount();
                float deltaTime = float(thisTick - lastTick) * 0.001f;
                app->Update(deltaTime);
                lastTick = thisTick;

                // Set view 0 default viewport.
                bgfx::setViewRect(0, 0, 0, uint16_t(mWidth), uint16_t(mHeight));
                app->Render(mAspectRatio);

                // BeginGUIFrame(0, 0, 0, 0, mWidth, mHeight);
                app->OnGUI();
                // EndGUIFrame();
            }

            imguiDestroy();
            app->Shutdown();

            // Shutdown bgfx.
            bgfx::shutdown();

            while (bgfx::RenderFrame::NoContext != bgfx::renderFrame())
            {
            };

            delete app;
            DestroyWindow(mHWND[kMainWindowIndex]);

            return -1;
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
            int32_t newWidth = newrect.right - newrect.left;
            int32_t newHeight = newrect.bottom - newrect.top;

            if (!windowFrame)
            {
                float aspectRatio = 1.0f / mAspectRatio;
                newWidth = bx::uint32_max(kAppWidth / 4, newWidth);
                newHeight = uint32_t(float(newWidth) * aspectRatio);

                left = newrect.left + (newrect.right - newrect.left - newWidth) / 2;
                top = newrect.top + (newrect.bottom - newrect.top - newHeight) / 2;
            }

            SetWindowPos(hwnd, HWND_TOP, left, top, newWidth, newHeight, SWP_SHOWWINDOW);

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
    static Context sContext;
#pragma region Windows Event
    LRESULT CALLBACK Context::WndProc(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam)
    {
        return sContext.ProcessEvent(hwnd, id, wparam, lparam);
    }

    LRESULT CALLBACK Context::ProcessEvent(HWND hwnd, UINT id, WPARAM wparam, LPARAM lparam)
    {
        if (!mInit)
        {
            return DefWindowProcW(hwnd, id, wparam, lparam);
        }
        switch (id)
        {
        case WM_USER_WINDOW_CREATE:
        {
            Msg *msg = (Msg *)lparam;
            HWND newHwnd = CreateWindowW(L"Tiga", UTF8ToUTF16(msg->mTitle.c_str()).data(),
                                         WS_OVERLAPPEDWINDOW | WS_VISIBLE, msg->mX, msg->mY,
                                         msg->mWidth, msg->mHeight, NULL, NULL,
                                         (HINSTANCE)GetModuleHandle(NULL), 0);
            Adjust(hwnd, msg->mWidth, msg->mHeight, true);
            Clear(hwnd);

            mHWND[wparam] = hwnd;
            mFlags[wparam] = msg->mFlags;
            WindowHandle handle = {(uint16_t)wparam};
            mEventQueue.Post(SizeEvent::GetEvent(handle, msg->mWidth, msg->mHeight));
            mEventQueue.Post(WindowEvent::GetEvent(handle, newHwnd));

            delete msg;
        }
        break;
        case WM_USER_WINDOW_DESTROY:
        {
            WindowHandle handle = {(uint16_t)wparam};
            mEventQueue.Post(WindowEvent::GetEvent(handle));
            DestroyWindow(mHWND[wparam]);
            mHWND[wparam] = 0;

            if (handle.idx == 0)
            {
                mExit = true;
                mEventQueue.Post(Event::GetEvent(Event::Exit));
            }
        }
        break;
        case WM_USER_WINDOW_SET_TITLE:
        {
            Msg *msg = (Msg *)lparam;
            SetWindowTextW(mHWND[wparam], UTF8ToUTF16(msg->mTitle.c_str()).data());
            delete msg;
        }
        break;
        case WM_USER_WINDOW_SET_FLAGS:
        {
            Msg *msg = (Msg *)lparam;
            if (msg->mFlagsEnabled)
            {
                mFlags[wparam] |= msg->mFlags;
            }
            else
            {
                mFlags[wparam] &= ~msg->mFlags;
            }

            delete msg;
        }
        break;
        case WM_USER_WINDOW_SET_POS:
        {
            Msg *msg = (Msg *)lparam;
            SetWindowPos(mHWND[wparam], 0, msg->mX, msg->mY, 0, 0,
                         SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE);
            delete msg;
        }
        break;
        case WM_USER_WINDOW_SET_SIZE:
        {
            uint32_t width = GET_X_LPARAM(lparam);
            uint32_t height = GET_Y_LPARAM(lparam);
            Adjust(mHWND[kMainWindowIndex], width, height, true);
        }
        break;
        case WM_USER_WINDOW_TOGGLE_FRAME:
        {
            if (mFrame)
            {
                mOldWidth = mWidth;
                mOldHeight = mHeight;
            }
            Adjust(mHWND[wparam], mOldWidth, mOldHeight, !mFrame);
        }
        break;
        case WM_USER_WINDOW_MOUSE_LOCK:
        {
            SetMouseLock(mHWND[wparam], !!lparam);
        }
        break;
        case WM_DESTROY:
            break;

        case WM_QUIT:
        case WM_CLOSE:
            DestroyWindow(FindHandle(hwnd));
            // Don't process message. Window will be destroyed later.
            return 0;

        case WM_SIZING:
        {
            WindowHandle handle = FindHandle(hwnd);
            if (IsWindowHandleValid(handle) && WINDOW_FLAG_ASPECT_RATIO & mFlags[handle.idx])
            {
                RECT &rect = *(RECT *)lparam;
                uint32_t width = rect.right - rect.left - mFrameWidth;
                uint32_t height = rect.bottom - rect.top - mFrameHeight;

                // Recalculate size according to aspect ratio
                switch (wparam)
                {
                case WMSZ_LEFT:
                case WMSZ_RIGHT:
                {
                    float aspectRatio = 1.0f / mAspectRatio;
                    width = bx::uint32_max(kAppWidth / 4, width);
                    height = uint32_t(float(width) * aspectRatio);
                }
                break;

                default:
                {
                    height = bx::uint32_max(kAppHeight / 4, height);
                    width = uint32_t(float(height) * mAspectRatio);
                }
                break;
                }

                // Recalculate position using different anchor points
                switch (wparam)
                {
                case WMSZ_TOPLEFT:
                    rect.left = rect.right - width - mFrameWidth;
                    rect.top = rect.bottom - height - mFrameHeight;
                    break;

                case WMSZ_TOP:
                case WMSZ_TOPRIGHT:
                    rect.right = rect.left + width + mFrameWidth;
                    rect.top = rect.bottom - height - mFrameHeight;
                    break;

                case WMSZ_LEFT:
                case WMSZ_BOTTOMLEFT:
                    rect.left = rect.right - width - mFrameWidth;
                    rect.bottom = rect.top + height + mFrameHeight;
                    break;

                default:
                    rect.right = rect.left + width + mFrameWidth;
                    rect.bottom = rect.top + height + mFrameHeight;
                    break;
                }

                mEventQueue.Post(SizeEvent::GetEvent(FindHandle(hwnd), width, height));
            }
        }
            return 0;
        case WM_SIZE:
        {
            WindowHandle handle = FindHandle(hwnd);
            if (IsWindowHandleValid(handle))
            {
                uint32_t width = GET_X_LPARAM(lparam);
                uint32_t height = GET_Y_LPARAM(lparam);

                mWidth = width;
                mHeight = height;
                mEventQueue.Post(SizeEvent::GetEvent(handle, width, height));
            }
        }
        break;

        case WM_SYSCOMMAND:
        {
            switch (wparam)
            {
            case SC_MINIMIZE:
            case SC_RESTORE:
            {
                HWND parent = GetWindow(hwnd, GW_OWNER);
                if (NULL != parent)
                {
                    PostMessage(parent, id, wparam, lparam);
                }
            }
            }
        }
        break;
        case WM_MOUSEMOVE:
        {
            int32_t mx = GET_X_LPARAM(lparam);
            int32_t my = GET_Y_LPARAM(lparam);

            if (hwnd == mMouseLock)
            {
                mx -= mMouseX;
                my -= mMouseY;

                if (0 == mx && 0 == my)
                {
                    break;
                }

                SetMousePos(hwnd, mMouseX, mMouseY);
            }

            mEventQueue.Post(MouseEvent::GetEvent(FindHandle(hwnd), mx, my, mMouseZ));
        }
        break;

        case WM_MOUSEWHEEL:
        {
            POINT pt = {GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)};
            ScreenToClient(hwnd, &pt);
            int32_t mx = pt.x;
            int32_t my = pt.y;
            mMouseZ += GET_WHEEL_DELTA_WPARAM(wparam) / WHEEL_DELTA;
            mEventQueue.Post(MouseEvent::GetEvent(FindHandle(hwnd), mx, my, mMouseZ));
        }
        break;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        {
            MouseCapture(hwnd, id == WM_LBUTTONDOWN);
            int32_t mx = GET_X_LPARAM(lparam);
            int32_t my = GET_Y_LPARAM(lparam);
            mEventQueue.Post(MouseEvent::GetEvent(FindHandle(hwnd), mx, my, mMouseZ, Input::MouseButton::Left, id == WM_LBUTTONDOWN));
        }
        break;
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MBUTTONDBLCLK:
        {
            MouseCapture(hwnd, id == WM_LBUTTONDOWN);
            int32_t mx = GET_X_LPARAM(lparam);
            int32_t my = GET_Y_LPARAM(lparam);
            mEventQueue.Post(MouseEvent::GetEvent(FindHandle(hwnd), mx, my, mMouseZ, Input::MouseButton::Middle, id == WM_LBUTTONDOWN));
        }
        break;
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
        {
            MouseCapture(hwnd, id == WM_LBUTTONDOWN);
            int32_t mx = GET_X_LPARAM(lparam);
            int32_t my = GET_Y_LPARAM(lparam);
            mEventQueue.Post(MouseEvent::GetEvent(FindHandle(hwnd), mx, my, mMouseZ, Input::MouseButton::Right, id == WM_LBUTTONDOWN));
        }
        break;
        case WM_CHAR:
        {
            WCHAR utf16[2] = {(WCHAR)wparam};
            uint8_t utf8[4] = {};

            if (utf16[0] >= 0xD800 && utf16[0] <= 0xDBFF)
            {
                mSurrogate = utf16[0];
            }
            else
            {
                int utf16Len;
                if (utf16[0] >= 0xDC00 && utf16[0] <= 0xDFFF)
                {
                    utf16[1] = utf16[0];
                    utf16[0] = mSurrogate;
                    mSurrogate = 0;
                    utf16Len = 2;
                }
                else
                {
                    utf16Len = 1;
                }
                uint8_t len = (uint8_t)WideCharToMultiByte(CP_UTF8, 0, utf16, utf16Len, (LPSTR)utf8, BX_COUNTOF(utf8), NULL, NULL);
                if (0 != len)
                {
                    WindowHandle handle = FindHandle(hwnd);
                    mEventQueue.Post(CharEvent::GetEvent(handle, len, utf8));
                }
            }
        }
        break;
        case WM_DROPFILES:
        {
            HDROP drop = (HDROP)wparam;
            char tmp[bx::kMaxFilePath];
            WCHAR utf16[bx::kMaxFilePath];
            uint32_t result = DragQueryFileW(drop, 0, utf16, bx::kMaxFilePath);
            BX_UNUSED(result);
            WideCharToMultiByte(CP_UTF8, 0, utf16, -1, tmp, bx::kMaxFilePath, NULL, NULL);
            mEventQueue.Post(DropFileEvent::GetEvent(FindHandle(hwnd), tmp));
        }
        break;
        default:
            break;
        }

        return DefWindowProcW(hwnd, id, wparam, lparam);
    }

#pragma endregion Windows Event

#pragma region WindowHandle

    WindowHandle CreateWindowHandle(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t flags, const char *title)
    {
        bx::MutexScope scope(sContext.mLock);
        WindowHandle handle = {sContext.mWindowAlloc.alloc()};

        if (handle.idx != UINT16_MAX)
        {
            Msg *msg = new Msg;
            msg->mX = x;
            msg->mY = y;
            msg->mWidth = width;
            msg->mHeight = height;
            msg->mTitle = title;
            msg->mFlags = flags;
            PostMessage(sContext.mHWND[kMainWindowIndex], WM_USER_WINDOW_CREATE, handle.idx, (LPARAM)msg);
        }
        return handle;
    }

    void DestroyWindow(WindowHandle handle)
    {
        if (handle.idx != UINT16_MAX)
        {
            PostMessage(sContext.mHWND[kMainWindowIndex], WM_USER_WINDOW_DESTROY, handle.idx, 0);
            bx::MutexScope scope(sContext.mLock);
            sContext.mWindowAlloc.free(handle.idx);
        }
    }

    void SetWindowPos(WindowHandle handle, int32_t x, int32_t y)
    {
        Msg *msg = new Msg;
        msg->mX = x;
        msg->mY = y;
        PostMessage(sContext.mHWND[kMainWindowIndex], WM_USER_WINDOW_SET_POS, handle.idx, (LPARAM)msg);
    }

    void SetWindowSize(WindowHandle handle, uint32_t width, uint32_t height)
    {
        PostMessage(sContext.mHWND[kMainWindowIndex], WM_USER_WINDOW_SET_SIZE, handle.idx, (width << 16) || (width & 0xffff));
    }

    void SetWindowTitle(WindowHandle handle, const char *title)
    {
        Msg *msg = new Msg;
        msg->mTitle = title;
        PostMessage(sContext.mHWND[kMainWindowIndex], WM_USER_WINDOW_SET_TITLE, handle.idx, (LPARAM)msg);
    }

    void SetWindowFlags(WindowHandle handle, uint32_t flags, bool enabled)
    {
        Msg *msg = new Msg;
        msg->mFlags = flags;
        msg->mFlagsEnabled = enabled;
        PostMessage(sContext.mHWND[kMainWindowIndex], WM_USER_WINDOW_SET_FLAGS, handle.idx, (LPARAM)msg);
    }

    void ToggleFullscreen(WindowHandle handle)
    {
        PostMessage(sContext.mHWND[kMainWindowIndex], WM_USER_WINDOW_TOGGLE_FRAME, handle.idx, 0);
    }

    void SetMouseLock(WindowHandle handle, bool lock)
    {
        PostMessage(sContext.mHWND[kMainWindowIndex], WM_USER_WINDOW_MOUSE_LOCK, handle.idx, lock);
    }
#pragma endregion WindowHandle
    int32_t Run(Application *app, int argc, const char *const *argv)
    {
        return sContext.Run(app, argc, argv);
    }
} // namespace Tiga

#endif