#ifndef _INPUT_H_
#define _INPUT_H_
#include <iostream>

#include "Key.h"
#include "MouseButton.h"
namespace Tiga
{
#define INPUT_BINDING_END                                      \
    {                                                          \
        Key::None, Modifier::None, 0, NULL, NULL \
    }

    typedef void (*InputBindingFn)(const void *userData);
    struct InputBinding
    {
        Key::Enum mKey;
        uint8_t mModifiers;
        uint8_t mFlags;
        InputBindingFn mFn;
        const void *mUserData;

        void Set(Key::Enum key, uint8_t modifiers, uint8_t flags, InputBindingFn fn, const void *userData)
        {
            mKey = key;
            mModifiers = modifiers;
            mFlags = flags;
            mFn = fn;
            mUserData = userData;
        }

        void End()
        {
            mKey = Key::None;
            mModifiers = Modifier::None;
            mFlags = 0;
            mFn = NULL;
            mUserData = NULL;
        }
    };

    ///
    void InputInit();

    ///
    void InputShutdown();

    ///
    void InputAddBindings(const char *_name, const InputBinding *_bindings);

    ///
    void InputRemoveBindings(const char *_name);

    ///
    void InputProcess();

    ///
    void InputSetKeyState(Key::Enum _key, uint8_t _modifiers, bool _down);

    ///
    bool inputGetKeyState(Key::Enum _key, uint8_t *_modifiers = NULL);

    ///
    uint8_t InputGetModifiersState();

    /// Adds single UTF-8 encoded character into input buffer.
    void InputChar(uint8_t _len, const uint8_t _char[4]);

    /// Returns single UTF-8 encoded character from input buffer.
    const uint8_t *InputGetChar();

    /// Flush internal input buffer.
    void InputCharFlush();

    ///
    void InputSetMouseResolution(uint16_t _width, uint16_t _height);

    ///
    void InputSetMousePos(int32_t _mx, int32_t _my, int32_t _mz);

    ///
    void InputSetMouseButtonState(MouseButton::Enum _button, uint8_t _state);

    ///
    void InputSetMouseLock(bool _lock);

    ///
    void InputGetMouse(float _mouse[3]);

    ///
    bool InputIsMouseLocked();

} // namespace Tiga

#endif