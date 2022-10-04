#include "helloworld.h"

void HelloWorld::onRender(float _inAspectRatio)
{
    // default 8 x 16 font
    bgfx::dbgTextClear();

    bgfx::dbgTextPrintf(bx::max<uint16_t>(uint16_t(m_width / 2 / 8), 20),
                        bx::max<uint16_t>(uint16_t(m_height / 2 / 16), 6),
                        0x0f, "Hello World");
}
