#ifndef _SUSPENDEVENT_H_
#define _SUSPENDEVENT_H_
#include "Event.h"
#include "../Utils/BgfxUtils.h"
#include "../Enums/Suspend.h"

namespace Tiga
{
    struct SuspendEvent : public Event
    {
        Suspend::Enum mState;

        SuspendEvent(WindowHandle handle) : Event(Event::Suspend, handle) {}

        static SuspendEvent *GetEvent(WindowHandle handle, Suspend::Enum state)
        {
            SuspendEvent *ev = BX_NEW(GetAllocator(), SuspendEvent)(handle);
            ev->mState = state;
            return ev;
        }
    };

} // namespace Tiga

#endif //_SUSPENDEVENT_H_