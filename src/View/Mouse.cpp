#include "Mouse.hpp"

namespace tgl
{
	void Mouse::shift(int32_t _LastX, int32_t _LastY) noexcept
	{
		mUpdated = true;

		lastX = x;
		lastY = y;

		x += _LastX;
		y += _LastY;
	}

	void Mouse::move(int32_t _LastX, int32_t _LastY, int64_t _State) noexcept
	{
		mUpdated = true;

		lastX = x;
		lastY = y;

		x = _LastX;
		y = _LastY;
	}

	bool Mouse::get_update_state() noexcept
	{
		return mUpdated;
	}

	std::pair<int32_t, int32_t> Mouse::get_shift() noexcept
	{
		mUpdated = false;
		return { x - lastX, y - lastY };
	}
}