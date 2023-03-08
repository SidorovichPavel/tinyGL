#include "Mouse.hpp"

namespace tgl
{
	void Mouse::shift(int32_t _LastX, int32_t _LastY) noexcept
	{
		mUpdated = true;

		mLastX = mX;
		mLastY = mY;

		mX += _LastX;
		mY += _LastY;
	}


	bool Mouse::is_updated() noexcept
	{
		return mUpdated;
	}

	std::pair<int32_t, int32_t> Mouse::get_shift() noexcept
	{
		mUpdated = false;
		return { mX - mLastX, mY - mLastY };
	}

	void Mouse::move(int32_t _LastX, int32_t _LastY, int64_t _State) noexcept
	{
		mUpdated = true;

		mLastX = mX;
		mLastY = mY;

		mX = _LastX;
		mY = _LastY;
	}

	void Mouse::lb_down(int64_t _State, uint16_t _X, uint16_t _Y) noexcept
	{
		mLBPress = true;
	}

	void Mouse::lb_up(int64_t _State, uint16_t _X, uint16_t _Y) noexcept
	{
		mLBPress = false;
	}

	void Mouse::rb_down(int64_t _State, uint16_t _X, uint16_t _Y) noexcept
	{
		mRBPress = true;
	}

	void Mouse::rb_up(int64_t _State, uint16_t _X, uint16_t _Y) noexcept
	{
		mRBPress = false;
	}

}