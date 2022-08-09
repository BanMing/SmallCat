#ifndef _INPUT_H_
#define _INPUT_H_
#include <iostream>
#include <string>
#include <map>

#include "Inputkeyboard.h"
#include "InputMouse.h"
#include "InputBinding.h"

namespace Tiga
{
    namespace Input
    {
        struct Input
        {
            typedef std::map<std::string, const InputBinding *> InputBindingMap;
            InputBindingMap mInputBindingsMap;
            InputKeyboard mKeyboard;
            InputMouse mMouse;

            Input() {}
            ~Input() {}

            void AddBindings(const char *name, const InputBinding *bindings)
            {
                mInputBindingsMap.insert(std::make_pair(std::string(name), bindings));
            }

            void RemoveBindings(const char *name)
            {
                InputBindingMap::iterator it = mInputBindingsMap.find(std::string(name));
                if (it != mInputBindingsMap.end())
                {
                    mInputBindingsMap.erase(it);
                }
            }

            void Process(const InputBinding *bindings)
            {
                for (const InputBinding *binding = bindings; binding->mKey != Key::None; binding++)
                {
                    uint8_t modifiers;
                    bool down = InputKeyboard::DecodeKeyState(mKeyboard.mKey[binding->mKey], modifiers);

                    if (binding->mFlags == 1)
                    {
                        if (down)
                        {
                            if (modifiers == binding->mModifiers && !mKeyboard.mOnce[binding->mKey])
                            {
                                if (NULL == binding->mFn)
                                {
                                    // cmd execit
                                }
                                else
                                {
                                    binding->mFn(binding->mUserData);
                                }
                                mKeyboard.mOnce[binding->mKey] = true;
                            }
                        }
                        else
                        {
                            mKeyboard.mOnce[binding->mKey] = false;
                        }
                    }
                    else
                    {
                        if (down && modifiers == binding->mModifiers)
                        {
                            if (NULL == binding->mFn)
                            {
                                // cmd execit
                            }
                            else
                            {
                                binding->mFn(binding->mUserData);
                            }
                        }
                    }
                }
            }

            void Process()
            {
                for (InputBindingMap::const_iterator it = mInputBindingsMap.begin(); it != mInputBindingsMap.end(); it++)
                {
                    Process(it->second);
                }
            }

            void Reset()
            {
                mMouse.Reset();
                mKeyboard.Reset();
            }
        };
    } // namespace Input
} // namespace Tiga

#endif //_INPUT_H_