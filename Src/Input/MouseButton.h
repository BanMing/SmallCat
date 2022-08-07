#ifndef _MOUSEBUTTON_H_
#define _MOUSEBUTTON_H_
namespace Tiga
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
} // namespace Tiga
#endif