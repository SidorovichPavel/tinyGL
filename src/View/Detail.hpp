#pragma once

#include <memory>

#include "../Utility/utility.hpp"
#include "../Event/Event.hpp"
#include "Style.hpp"
#include "Mouse.hpp"
#include "ViewBase.hpp"
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

		class WinHandler : public tgl::detail::ViewBase
		{
			HWND mHandle;
			HDC mDevice_context;
			HGLRC mGL_resource_content;
			RECT mWinGlobalSize;

			PAINTSTRUCT mPS;
			RECT mClientRect;
			HDC mDCHandle, mCompatibleDCHandle;
			HBITMAP mCompatibleBitMapHandle;
			HBRUSH mBrushHandle;
			HCURSOR mCursor;

			UINT mRawInputSize;
			HRAWINPUT mRawInputHandle;
			std::vector<uint8_t> mRawInputData;

			detail::Events mEvents;
			
			static LRESULT CALLBACK GeneralProc(HWND, UINT, WPARAM, LPARAM) noexcept;
			LRESULT WinProc(HWND, UINT, WPARAM, LPARAM) noexcept;

		public:
			WinHandler(const Style* _Style_Ptr);
			virtual ~WinHandler();
			
			detail::Events& get_events() noexcept;

			void init_opengl();
			void enable_opengl_context() noexcept;
			void swap_buffers() noexcept;
			
			void mouse_raw_input(bool _Mode);

			void show_cursor(bool _Mode) noexcept;
			void set_title(const std::string& title) noexcept;

			using handle_t = win::HWND;
			handle_t get_handle() noexcept;
			void redraw() noexcept;
			void close() noexcept;
			void destroy() noexcept;

		private:
			void send_message(uint32_t _Msg, uint64_t _WParam, int64_t _LParam) noexcept;
			void post_message(uint32_t _Msg, uint64_t _WParam, int64_t _LParam) noexcept;
		};
	}
	using Handler = win::WinHandler;
	#endif // __WIN32

}

