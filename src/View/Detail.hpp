#pragma once

#include <memory>

#include "../Utility/utility.hpp"
#include "../Event/Event.hpp"
#include "../Style/Style.hpp"
#include "Mouse.hpp"
#include "ViewTypes.hpp"

namespace tgl
{
#ifdef _WIN32
	namespace win {
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif

	#include <Windows.h>
	#include <windowsx.h>
	#include <WinUser.h>

		class WinHandler
		{
			HWND mHandle;
			HDC mDevice_context;
			HGLRC mGL_resource_content;

			static LRESULT CALLBACK GenProc(HWND, UINT, WPARAM, LPARAM) noexcept;
			LRESULT WinProc(HWND, UINT, WPARAM, LPARAM) noexcept;

			bool mMouseRawInput;
			UINT mRawInputSize;
			HRAWINPUT mRawInputHandle;
			std::vector<uint8_t> mRawInputData;

			detail::Events mEvents;
			
		protected:
			bool mIsOpen;
			RECT mWinGlobalSize;
			int mWidth, mHeight;
			int mScreenWidth, mScreenHeight;
		public:

			WinHandler(const Style* _Style_Ptr);
			~WinHandler();

			void init_opengl();
			void enable_opengl_context() noexcept;
			void mouse_raw_input(bool _Mode = true);
			void show_cursor(bool mode) noexcept;
			void set_title(const std::string& title) noexcept;
			void send_message(uint32_t _Msg, uint64_t _WParam, int64_t _LParam) noexcept;
			void post_message(uint32_t _Msg, uint64_t _WParam, int64_t _LParam) noexcept;
			void invalidate_rect() noexcept;
			void swap_buffers() noexcept;
			void destroy() noexcept;

			void close() noexcept;

			detail::Events& events() noexcept;
		};
	}
	using Handler = win::WinHandler;
#endif // __WIN32

}

