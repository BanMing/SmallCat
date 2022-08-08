#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_
#include <bx/allocator.h>
#include <bx/ringbuffer.h>
#include <iostream>

#include "../Contanst/AppConst.h"
#include "Key.h"
namespace Tiga
{
    struct InputKeyboard
    {
        uint32_t mKey[kMaxKeyCount];
        bool mOnce[kMaxKeyCount];

        bx::RingBufferControl mRing;
        uint8_t mChar[kMaxKeyCount];

        InputKeyboard() : mRing(BX_COUNTOF(mChar) - 4) {}

        void Reset()
        {
            bx::memSet(mKey, 0, sizeof(mKey));
            bx::memSet(mOnce, 0xff, sizeof(mOnce));
        }

        static uint32_t EncodeKeyState(uint8_t modifiers, bool down)
        {
            uint32_t state = 0;
            state |= uint32_t(down ? modifiers : 0) << 16;
            state |= uint32_t(down) << 8;
            return state;
        }

        static bool DecodeKeyState(uint32_t state, uint8_t &modifiers)
        {
            modifiers = (state >> 16) & 0xff;
            return 0 != ((state >> 8) & 0xff);
        }

        void SetKeyState(Key::Enum key, uint8_t modifiers, bool down)
        {
            mKey[key] = EncodeKeyState(modifiers, down);
            mOnce[key] = false;
        }

        bool GetKeyState(Key::Enum key, uint8_t *modifiers)
        {
            uint8_t modif;
            modifiers = NULL == modifiers ? &modif : modifiers;

            return DecodeKeyState(mKey[key], *modifiers);
        }

        uint8_t GetModifiersState()
        {
            uint8_t modifiers;
            for (uint32_t i = 0; i < Key::Count; i++)
            {
                modifiers |= (mKey[i] >> 16) & 0xff;
            }

            return modifiers;
        }

        void PushChar(uint8_t len, const uint8_t chars[4])
        {
            for (uint32_t i = mRing.reserve(4); i < len; i = mRing.reserve(4))
            {
                PopChar();
            }
            bx::memCopy(&mChar[mRing.m_current], chars, 4);
            mRing.commit(4);
        }

        const uint8_t *PopChar()
        {
            if (0 < mRing.available())
            {
                uint8_t *utf8 = &mChar[mRing.m_read];
                mRing.consume(4);
                return utf8;
            }
            return NULL;
        }

        void CharFlush()
        {
            mRing.m_current = 0;
            mRing.m_write = 0;
            mRing.m_read = 0;
        }
    };
} // namespace Tiga

#endif //_INPUTKEYBOARD_H_