#ifndef _WINDOW_STATE_H_
#define _WINDOW_STATE_H_
#include <iostream>
#include <bx/filepath.h>

#include "WindowHandle.h"

namespace Tiga
{
    struct WindowState
    {
        WindowHandle mHandle;
        uint32_t mWidth;
        uint32_t mHeight;
        void *mNwh;
        bx::FilePath mDropFile;
    };

    bool ProcessWindowEvents(WindowState &state, uint32_t &debug, uint32_t &reset);

} // namespace Tiga

#endif //_WINDOW_STATE_H_