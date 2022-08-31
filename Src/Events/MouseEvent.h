#ifndef _MOUSEEVENT_H_
#define _MOUSEEVENT_H_
#include "Event.h"
#include "../Input/MouseButton.h"

namespace Tiga
{
    struct MouseEvent : public Event
    {
        int32_t mMouseX;
        int32_t mMouseY;
        int32_t mMouseZ;
        Input::MouseButton::Enum mButton;
        bool mIsDown;
        bool mIsMove;

        MouseEvent(WindowHandle handle) : Event(Event::Mouse, handle) {}

        static MouseEvent *GetEvent(WindowHandle handle, int32_t mouseX, int32_t mouseY, int32_t mouseZ, Input::MouseButton::Enum button, bool isDown)
        {
            MouseEvent *ev = BX_NEW(GetAllocator(), MouseEvent)(handle);
            ev->mMouseX = mouseX;
            ev->mMouseY = mouseY;
            ev->mMouseZ = mouseZ;
            ev->mButton = button;
            ev->mIsMove = false;
            ev->mIsDown = isDown;
            return ev;
        }

        static MouseEvent *GetEvent(WindowHandle handle, int32_t mouseX, int32_t mouseY, int32_t mouseZ)
        {
            MouseEvent *ev = BX_NEW(GetAllocator(), MouseEvent)(handle);
            ev->mMouseX = mouseX;
            ev->mMouseY = mouseY;
            ev->mMouseZ = mouseZ;
            ev->mButton = Input::MouseButton::None;
            ev->mIsMove = false;
            ev->mIsDown = false;
            return ev;
        }
    };

} // namespace Tiga

#endif //_MOUSEEVENT_H_