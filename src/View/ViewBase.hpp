#pragma once

#include <stdint.h>
#include <xutility>

namespace tgl
{
	namespace detail
	{

		class ViewBase
		{
			
		public:

			virtual void init_opengl() = 0;
			virtual void enable_opengl_context() noexcept = 0;
			virtual void swap_buffers() noexcept = 0;

			std::pair<int32_t, int32_t> get_size() const noexcept;
			float get_ratio() const noexcept;
			bool is_open() const noexcept;

		protected:
			ViewBase(int32_t _Width, int32_t _Height, int32_t _ScreenWidth, int32_t _ScreenHeight) noexcept;
			ViewBase() noexcept;
			virtual ~ViewBase();

			bool mIsOpen;
			int32_t mWidth, mHeight;
			int32_t mScreenWidth, mScreenHeight;
		};
	}
}