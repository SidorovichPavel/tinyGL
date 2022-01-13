#pragma once

#include <cstdint>
#include <xutility>

namespace tgl
{
	namespace detail
	{
		template <class Handler>
		class MouseBase : public Handler
		{
			using base = Handler;
		public:
			MouseBase(int32_t _x = 0, int32_t _y = 0) noexcept : Handler(_x, _y)
			{};
			~MouseBase() {};
			MouseBase(const MouseBase&) = default;
			MouseBase(MouseBase&&) = default;

			int32_t dx() noexcept
			{
				return base::x - base::lastX;
			};
			int32_t dy() noexcept
			{
				return base::y - base::lastY;
			};

			MouseBase& operator=(const MouseBase&) = default;
			MouseBase& operator=(MouseBase&&) = default;
		};
	}

	#ifdef _WIN32
	namespace win
	{
		#include <Windows.h>
		class WinMouse
		{
		protected:
			int32_t x, lastX;
			int32_t y, lastY;
			bool lb_press;
			bool rb_press;
		public:
			WinMouse(int32_t _x = 0, int32_t _y = 0) noexcept :
				x(_x), lastX(0),
				y(_y), lastY(0),
				lb_press(false),
				rb_press(false)
			{}
			~WinMouse() {}

			void raw_input(unsigned short _Flags, int32_t _LastX, int32_t _LastY) noexcept;
		};

	}
	using Mouse = detail::MouseBase<win::WinMouse>;
	#endif
}