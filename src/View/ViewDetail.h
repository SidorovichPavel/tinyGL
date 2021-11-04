#pragma once

#include <stdexcept>
#include <memory>

#include <src/Event/Event.h>
#include <src/View/Mouse.h>
#include <src/Style/Style.h>
#include <src/View/ViewTypes.h>

namespace tgl::win
{
#ifdef _WIN32
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

		Mouse& mVirtualMouse;
		std::unique_ptr<detail::Events> mEvents;
	public:
		bool mIsOpen;
		RECT mWinGlobalSize;
		int mWidth, mHeight;

		WinHandler(const int width, const int height, const std::wstring& title, Mouse& _VMouse);
		~WinHandler();

		void init_opengl();
		void enale_opengl_context() noexcept;
		void mouse_raw_input(bool _Mode = true);
		void show_cursor(bool mode) noexcept;
		void send_message(uint32_t _Msg, uint64_t _WParam, int64_t _LParam) noexcept;
		void post_message(uint32_t _Msg, uint64_t _WParam, int64_t _LParam) noexcept;
		void invalidate_rect() noexcept;
		void swap_buffers() noexcept;
		void destroy() noexcept;

		void close() noexcept;

		detail::Events& events() noexcept;
	};



#endif // __WIN32
}

