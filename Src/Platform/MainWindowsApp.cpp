#include <bgfx/platform.h>

#if BX_PLATFORM_WINDOWS
#include <bx/mutex.h>
#include <bx/handlealloc.h>
#include <bx/os.h>
#include <bx/thread.h>
#include <bx/timer.h>
#include <bx/uint32_t.h>

#include "WindowHandle.h"
#include "WindowState.h"

WindowHandle CreateWindow(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t flags = WINDOW_FLAG_NONE, const char *title = "")
{
    // bx::MutexScope scope
}

void DestroyWindow(WindowHandle handle){}
void SetWindowPos(WindowHandle handle, int32_t x, int32_t y){}
void SetWindowSize(WindowHandle handle, uint32_t width, uint32_t height){}
void SetWindowFlags(WindowHandle handle, uint32_t flags, bool enabled){}
void SetMouseLock(WindowHandle handle, bool lock){}
void SetCurrentDir(const char *dir){}
#endif