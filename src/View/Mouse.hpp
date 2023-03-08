#pragma once

#include <cstdint>
#include <xutility>

namespace tgl
{
	class Mouse
	{
	protected:
		int32_t mX, mLastX;
		int32_t mY, mLastY;
		bool mLBPress;
		bool mRBPress;
		bool mUpdated;
	public:
		Mouse(int32_t _x = 0, int32_t _y = 0) noexcept :
			mX(_x), mLastX(0),
			mY(_y), mLastY(0),
			mLBPress(false),
			mRBPress(false),
			mUpdated(false)
		{}
		~Mouse() {}

		virtual void shift(int32_t _ShiftX, int32_t _ShiftY) noexcept;
		bool is_updated() noexcept;
		std::pair<int32_t, int32_t> get_shift() noexcept;

		virtual void move(int32_t _LastX, int32_t _LastY, int64_t _State) noexcept;

		virtual void lb_down(int64_t _State, uint16_t _X, uint16_t _Y) noexcept;
		virtual void lb_up(int64_t _State, uint16_t _X, uint16_t _Y) noexcept;
		virtual void rb_down(int64_t _State, uint16_t _X, uint16_t _Y) noexcept;
		virtual void rb_up(int64_t _State, uint16_t _X, uint16_t _Y) noexcept;
	};
}