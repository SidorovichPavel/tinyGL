#pragma once

#include <src/Event/Event.h>

#ifdef _WIN32
namespace tgl::win
{
	#include <Windows.h>
	namespace detail
	{
		struct Events
		{
			/*desc
			* window handel for you)
			*/
			Event<void(HWND)> create;
			/*desc
			* x, y
			*/
			Event<void(uint16_t, uint16_t)> size;
			/*desc
			* first - The virtual - key code of the nonsystem key.See Virtual - Key Codes.
			* second - The repeat count, scan code,
			* extended-key flag, context code, previous key-state flag,
			* and transition-state flag, as shown following.
			*/
			Event<void(uint64_t, int64_t)> key_down;
			/*desc
			* first - The virtual-key code of the nonsystem key. See Virtual-Key Codes.
			* second - The repeat count, scan code, extended-key flag, context code,
			* previous key-state flag, and transition-state flag, as shown in the following table.
			*/
			Event<void(uint64_t, int64_t)> key_up;
			/*desc
			* first - x
			* second - y
			* third - key state... read MSDN
			*/
			Event<void(int32_t, int32_t, int64_t)> mouse_move;
			/*desc
			* first - key state
			* second - distance the wheel is rotated,
			* expressed in multiples or divisions of WHEEL_DELTA, which is 120.
			* A positive value indicates that the wheel was rotated forward, away from the user;
			* a negative value indicates that the wheel was rotated backward, toward the user.
			*/
			Event<void(uint16_t, int16_t, int32_t, int32_t)> mouse_wheel;
			/*
			* The xand y coordinates of the upper - left corner of the client area of the window.
			*/
			Event<void(int32_t, int32_t)>move;

			Event<void(const RAWINPUT*)> raw_input;

			Event<void(RECT*)> moving;

			Event<void(HDC dc)> paint;

			Event<void()> close;

			Event<void(unsigned short, int32_t, int32_t)> mouse_raw_input;
			/*desc
			* first - additional flags. read msdn
			* second x position
			* thirt y positon
			*/
			Event<void(int64_t, int32_t, int32_t)> mouse_lbutton_down;
			/*desc
			* first - additional flags. read msdn
			* second x position
			* thirt y positon
			*/
			Event<void(int64_t, int32_t, int32_t)> mouse_lbutton_up;
			/*desc
			* first - additional flags. read msdn
			* second x position
			* thirt y positon
			*/
			Event<void(int64_t, int32_t, int32_t)> mouse_rbutton_down;
			/*desc
			* first - additional flags. read msdn
			* second x position
			* thirt y positon
			*/
			Event<void(int64_t, int32_t, int32_t)> mouse_rbutton_up;
		};
	}
}
#endif // 
