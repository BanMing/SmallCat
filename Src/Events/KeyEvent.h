#ifndef _KEYEVENT_H_
#define _KEYEVENT_H_
#include "Event.h"
#include "../Input/Key.h"
#include "../Utils/BgfxUtils.h"

namespace Tiga
{
    struct KeyEvent : public Event
    {
        Input::Key::Enum mKey;
        uint8_t mModifiers;
        bool mIsDown;

        KeyEvent(WindowHandle handle) : Event(Event::Key, handle) {}

        static KeyEvent *GetEvent(WindowHandle handle, Input ::Key::Enum key, uint8_t modifiers, bool isDown)
        {
            KeyEvent *ev = BX_NEW(GetAllocator(), KeyEvent)(handle);
            ev->mKey = key;
            ev->mModifiers = modifiers;
            ev->mIsDown = isDown;
            return ev;
        }
    };

} // namespace Tiga

#endif //_KEYEVENT_H_