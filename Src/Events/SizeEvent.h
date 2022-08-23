#ifndef _SIZEEVENT_H_
#define _SIZEEVENT_H_
#include "Event.h"
#include "../Utils/BgfxUtils.h"

namespace Tiga
{
    struct SizeEvent : public Event
    {
        uint32_t mWidth;
        uint32_t mHeight;

        SizeEvent(WindowHandle handle) : Event(Event::Size, handle) {}

        static SizeEvent *GetEvent(WindowHandle handle, int32_t width, int32_t height)
        {
            SizeEvent *ev = BX_NEW(GetAllocator(), SizeEvent)(handle);
            ev->mWidth = width;
            ev->mHeight = height;
            return ev;
        }
    };

} // namespace Tiga

#endif //_SIZEEVENT_H_