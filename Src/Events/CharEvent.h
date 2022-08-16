#ifndef _CHAREVENT_H_
#define _CHAREVENT_H_
#include <bx/allocator.h>

#include "Event.h"
#include "../Utils/BgfxUtils.h"
namespace Tiga
{
    struct CharEvent : public Event
    {
        uint8_t mLen;
        uint8_t mChar[4];

        CharEvent(WindowHandle handle) : Event(Event::Axis, handle) {}
        
        static Event *GetEvent(WindowHandle handle, uint8_t len, uint8_t chars[4])
        {
            CharEvent *ev = BX_NEW(GetAllocator(), CharEvent)(handle);
            ev->mLen = len;
            bx::memCopy(ev->mChar, chars, 4);
            return ev;
        }
    };
} // namespace Tiga

#endif //_CHAREVENT_H_