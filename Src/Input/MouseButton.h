#ifndef _MOUSEBUTTON_H_
#define _MOUSEBUTTON_H_
namespace Tiga
{
    namespace Input
    {
        struct MouseButton
        {
            enum Enum
            {
                None,
                Left,
                Middle,
                Right,

                Count
            };
        };
    } // namespace Input
} // namespace Tiga
#endif