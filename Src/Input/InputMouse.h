#ifndef _INPUTMOUSE_H_
#define _INPUTMOUSE_H_

#include <bx/allocator.h>

#include "MouseButton.h"
#include "../Contanst/AppConst.h"

namespace Tiga
{
    struct InputMouse
    {
        int32_t mAbsolte[3];
        float mNorm[3];
        int32_t mWheel;
        uint8_t mButtons[MouseButton::Count];
        uint16_t mWidth;
        uint16_t mHeight;
        uint16_t mWheelDelta;
        bool mLock;

        InputMouse() : mWidth(kAppWidth), mHeight(kAppHeight), mWheelDelta(120), mLock(false) {}

        void Reset()
        {
            if (mLock)
            {
                mNorm[0] = 0.0f;
                mNorm[1] = 0.0f;
                mNorm[2] = 0.0f;
            }
            bx::memSet(mButtons, 0, sizeof(mButtons));
        }

        void SetResolution(uint16_t width, uint16_t height)
        {
            mWidth = width;
            mHeight = height;
        }

        void SetPos(int32_t mouseX, int32_t mouseY, int32_t mouseZ)
        {
            mAbsolte[0] = mouseX;
            mAbsolte[1] = mouseY;
            mAbsolte[2] = mouseZ;
            mNorm[0] = float(mouseX) / float(mWidth);
            mNorm[1] = float(mouseY) / float(mHeight);
            mNorm[2] = float(mouseZ) / float(mWheelDelta);
        }

        void SetButtonState(MouseButton::Enum button, uint8_t state)
        {
            mButtons[button] = state;
        }
    };
} // namespace Tiga

#endif //_INPUTMOUSE_H_