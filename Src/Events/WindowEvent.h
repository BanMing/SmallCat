#ifndef _WINDOWEVENT_H_
#define _WINDOWEVENT_H_
#include "Event.h"
#include "../Utils/BgfxUtils.h"

namespace Tiga
{
    struct WindowEvent : public Event
    {
        void *mNwh;

        WindowEvent(WindowHandle handle) : Event(Event::Window, handle) {}

        static WindowEvent *GetEvent(WindowHandle handle, void *nwh = NULL)
        {
            WindowEvent *ev = BX_NEW(GetAllocator(), WindowEvent)(handle);
            ev->mNwh = nwh;
            return ev;
        }
    };

} // namespace Tiga

#endif //_WINDOWEVENT_H_