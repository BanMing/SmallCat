#ifndef _EVENT_H_
#define _EVENT_H_

#include "../Platform/WindowHandle.h"

namespace Tiga
{
    struct Event
    {
        enum Enum
        {
            Axis,
            Char,
            Exit,
            Gamepad,
            Key,
            Mouse,
            Size,
            Window,
            Suspend,
            DropFile
        };

        Event::Enum mType;
        WindowHandle mHandle;

        Event(Enum type) : mType(type) { mHandle.idx = UINT16_MAX; }
        Event(Enum type, WindowHandle handle) : mType(type), mHandle(handle) {}
    };

} // namespace Tiga

#endif //_EVENT_H_