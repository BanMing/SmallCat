#include <bx/allocator.h>

#include "Input.h"
#include "InputSystem.h"
#include "../Utils/BgfxUtils.h"

namespace Tiga
{
    namespace Input
    {
        static Input *sInput;

        ///
        void InputInit()
        {
            sInput = BX_NEW(GetAllocator(), Input);
        }

        ///
        void InputShutdown()
        {
            BX_DELETE(GetAllocator(), sInput);
        }

        ///
        void InputAddBindings(const char *name, const InputBinding *bindings)
        {
            sInput->AddBindings(name, bindings);
        }

        ///
        void InputRemoveBindings(const char *name)
        {
            sInput->RemoveBindings(name);
        }

        ///
        void InputProcess()
        {
            sInput->Process();
        }

        ///
        void InputSetKeyState(Key::Enum key, uint8_t modifiers, bool down)
        {
            sInput->mKeyboard.SetKeyState(key, modifiers, down);
        }

        ///
        bool inputGetKeyState(Key::Enum key, uint8_t *modifiers = NULL)
        {
            return sInput->mKeyboard.GetKeyState(key, modifiers);
        }

        ///
        uint8_t InputGetModifiersState()
        {
            return sInput->mKeyboard.GetModifiersState();
        }

        /// Adds single UTF-8 encoded character into input buffer.
        void InputChar(uint8_t len, const uint8_t chars[4])
        {
            sInput->mKeyboard.PushChar(len, chars);
        }

        /// Returns single UTF-8 encoded character from input buffer.
        const uint8_t *InputGetChar()
        {
            return sInput->mKeyboard.PopChar();
        }

        /// Flush internal input buffer.
        void InputCharFlush()
        {
            sInput->mKeyboard.CharFlush();
        }

        ///
        void InputSetMouseResolution(uint16_t width, uint16_t height)
        {
            sInput->mMouse.SetResolution(width, height);
        }

        ///
        void InputSetMousePos(int32_t mouseX, int32_t mouseY, int32_t mouseZ)
        {
            sInput->mMouse.SetPos(mouseX, mouseY, mouseZ);
        }

        ///
        void InputSetMouseButtonState(MouseButton::Enum button, uint8_t state)
        {
            sInput->mMouse.SetButtonState(button, state);
        }

        ///
        void InputSetMouseLock(bool lock)
        {
            sInput->mMouse.mLock = lock;
            if (sInput->mMouse.mLock != lock)
            {
                sInput->mMouse.mLock = lock;
                // WindowHandle deaultWindow={0};
                // SetMouseLock(deaultWindow, lock);
                if (lock)
                {
                    sInput->mMouse.mNorm[0] = 0.0f;
                    sInput->mMouse.mNorm[1] = 0.0f;
                    sInput->mMouse.mNorm[2] = 0.0f;
                }
            }
        }

        ///
        void InputGetMouse(float mouse[3])
        {
            mouse[0] = sInput->mMouse.mNorm[0];
            mouse[1] = sInput->mMouse.mNorm[1];
            mouse[2] = sInput->mMouse.mNorm[2];

            sInput->mMouse.mNorm[0] = 0.0f;
            sInput->mMouse.mNorm[1] = 0.0f;
            sInput->mMouse.mNorm[2] = 0.0f;
        }

        ///
        bool InputIsMouseLocked()
        {
            return sInput->mMouse.mLock;
        }
    } // namespace Input
} // namespace Tiga
