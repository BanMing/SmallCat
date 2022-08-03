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

    WindowHandle CreateWindow(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t flags = WINDOW_FLAG_NONE, const char *title = "");
    void DestroyWindow(WindowHandle handle);
    void SetWindowPos(WindowHandle handle, int32_t x,int32_t y);
    void SetWindowSize(WindowHandle handle,uint32_t width,uint32_t height);
    void SetWindowFlags(WindowHandle handle,uint32_t flags,bool enabled);
    void SetMouseLock(WindowHandle handle,bool lock);
    void SetCurrentDir(const char* dir);
} // namespace Tiga

#endif //_WINDOW_HANDLE_H_