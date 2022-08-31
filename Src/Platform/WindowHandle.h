#ifndef _WINDOW_HANDLE_H_
#define _WINDOW_HANDLE_H_
#include <iostream>

#define WINDOW_FLAG_NONE UINT32_C(0x00000000)
#define WINDOW_FLAG_ASPECT_RATIO UINT32_C(0x00000001)
#define WINDOW_FLAG_FRAME UINT32_C(0x00000002)

namespace Tiga
{
    struct WindowHandle
    {
        uint16_t idx;
    };
	inline bool IsWindowHandleValid(WindowHandle handle)  { return UINT16_MAX != handle.idx; }

    WindowHandle CreateWindowHandle(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t flags = WINDOW_FLAG_NONE, const char *title = "");
    void DestroyWindow(WindowHandle handle);
    void SetWindowPos(WindowHandle handle, int32_t x, int32_t y);
    void SetWindowSize(WindowHandle handle, uint32_t width, uint32_t height);
    void SetWindowTitle(WindowHandle handle, const char* title);
    void SetWindowFlags(WindowHandle handle, uint32_t flags, bool enabled);
    void ToggleFullscreen(WindowHandle handle);
    void SetMouseLock(WindowHandle handle, bool lock);
} // namespace Tiga

#endif //_WINDOW_HANDLE_H_