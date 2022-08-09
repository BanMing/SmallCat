#ifndef _MOUSESTATE_H_
#define _MOUSESTATE_H_
#include "MouseButton.h"
#include <iostream>

namespace Tiga
{
	namespace Input
	{
		struct MouseState
		{
			MouseState()
				: mX(0), mY(0), mZ(0)
			{
				for (uint32_t ii = 0; ii < MouseButton::Count; ++ii)
				{
					mButtons[ii] = MouseButton::None;
				}
			}

			int32_t mX;
			int32_t mY;
			int32_t mZ;
			uint8_t mButtons[MouseButton::Count];
		};
	} // namespace Input
} // namespace Tiga
#endif