#ifndef _INPUTBINDING_H_
#define _INPUTBINDING_H_
#include <iostream>

#include "Key.h"
namespace Tiga
{
#define INPUT_BINDING_END                        \
    {                                            \
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

} // namespace Tiga

#endif //_INPUTBINDING_H_