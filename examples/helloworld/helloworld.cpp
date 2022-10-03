#include "helloworld.h"

void HelloWorld::onRender(float _inAspectRatio)
{
    // Use debug font to print information about this example.
    bgfx::dbgTextClear();
    // bgfx::dbgTextImage(
    //     bx::max<uint16_t>(uint16_t(m_width / 2 / 8), 20) - 20, bx::max<uint16_t>(uint16_t(m_height / 2 / 16), 6) - 6, 40, 12, s_logo, 160);
    bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");

    bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
    bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
}
