#ifndef _EVENT_H_
#define _EVENT_H_

#include "../Platform/WindowHandle.h"
#include "../Utils/BgfxUtils.h"

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

        static Event *GetEvent(Enum type)
        {
            Event *ev = BX_NEW(GetAllocator(), Event)(type);
            return ev;
        }
    };

} // namespace Tiga

#endif //_EVENT_H_