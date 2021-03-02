#include "Mouse.h"

namespace tgl
{

	Mouse::Mouse(int32_t _x, int32_t _y)
		:
		x(_x), lastX(0),
		y(_y), lastY(0),
		lb_press(false),
		rb_press(false)
	{}

	Mouse::~Mouse()
	{}

	int32_t Mouse::dx() noexcept
	{
		return x - lastX;
	}

	int32_t Mouse::dy() noexcept
	{
		return y - lastY;
	}

	Mouse& Mouse::operator=(const std::pair<int32_t, int32_t>& _Right)
	{
		lastX = x;
		lastY = y;
		x = _Right.first;
		y = _Right.second;
		return *this;
	}

	Mouse& Mouse::operator+=(const std::pair<int32_t, int32_t>& _Right)
	{
		lastX = x;
		lastY = y;
		x += _Right.first;
		y += _Right.second;
		return *this;
	}

	Mouse& Mouse::operator-=(const std::pair<int32_t, int32_t>& _Right)
	{
		lastX = x;
		lastY = y;
		x -= _Right.first;
		y -= _Right.second;
		return *this;
	}

}