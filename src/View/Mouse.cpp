#include "Mouse.hpp"

#ifdef _WIN32
namespace tgl::win
{
	void WinMouse::raw_input(unsigned short _Flags, int32_t _LastX, int32_t _LastY) noexcept
	{
		if (_Flags & MOUSE_MOVE_ABSOLUTE)
		{
			lastX = x;
			lastY = y;
			x = _LastX;
			y = _LastY;
		}
		else
		{
			lastX = x;
			lastY = y;
			x += _LastX;
			y += _LastY;
		}
	}
}

#endif