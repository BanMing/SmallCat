#ifndef _INPUTSYSTEM_H_
#define _INPUTSYSTEM_H_

#include <iostream>

#include "Key.h"
#include "MouseButton.h"
#include "InputBinding.h"
namespace Tiga
{
    namespace Input
    {
        ///
        void InputInit();

        ///
        void InputShutdown();

        ///
        void InputAddBindings(const char *name, const InputBinding *bindings);

        ///
        void InputRemoveBindings(const char *name);

        ///
        void InputProcess();

        ///
        void InputSetKeyState(Key::Enum key, uint8_t modifiers, bool down);

        ///
        bool inputGetKeyState(Key::Enum key, uint8_t *modifiers = NULL);

        ///
        uint8_t InputGetModifiersState();

        /// Adds single UTF-8 encoded character into input buffer.
        void InputChar(uint8_t len, const uint8_t chars[4]);

        /// Returns single UTF-8 encoded character from input buffer.
        const uint8_t *InputGetChar();

        /// Flush internal input buffer.
        void InputCharFlush();

        ///
        void InputSetMouseResolution(uint16_t width, uint16_t height);

        ///
        void InputSetMousePos(int32_t mouseX, int32_t mouseY, int32_t mouseZ);

        ///
        void InputSetMouseButtonState(MouseButton::Enum button, uint8_t state);

        ///
        void InputSetMouseLock(bool lock);

        ///
        void InputGetMouse(float mouse[3]);

        ///
        bool InputIsMouseLocked();
    } // namespace Input
} // namespace Tiga

#endif //_INPUTSYSTEM_H_
