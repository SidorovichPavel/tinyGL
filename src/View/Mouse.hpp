#pragma once

#include <cstdint>
#include <xutility>

namespace tgl
{
	class Mouse
	{
	protected:
		int32_t x, lastX;
		int32_t y, lastY;
		bool lb_press;
		bool rb_press;
		bool mUpdated;
	public:
		Mouse(int32_t _x = 0, int32_t _y = 0) noexcept :
			x(_x), lastX(0),
			y(_y), lastY(0),
			lb_press(false),
			rb_press(false),
			mUpdated(false)
		{}
		~Mouse() {}

		virtual void shift(int32_t _ShiftX, int32_t _ShiftY) noexcept;
		virtual void move(int32_t _LastX, int32_t _LastY, int64_t _State) noexcept;
		bool get_update_state() noexcept;
		std::pair<int32_t, int32_t> get_shift() noexcept;
	};
}