#pragma once

#include <cstdint>
#include <xutility>

namespace tgl
{

	class Mouse
	{
	public:
		int32_t x, lastX;
		int32_t y, lastY;

		Mouse(int32_t _x = 0, int32_t _y = 0);
		Mouse(const Mouse&) = delete;
		Mouse(Mouse&&) = delete;
		~Mouse();

		int32_t dx() noexcept;
		int32_t dy() noexcept;

		Mouse& operator=(const Mouse&) = default;
		Mouse& operator=(const std::pair<int32_t, int32_t>& _Right);
		
		Mouse& operator+=(const std::pair<int32_t, int32_t>& _Right);
		Mouse& operator-=(const std::pair< int32_t, int32_t>& _Right);
	};

}