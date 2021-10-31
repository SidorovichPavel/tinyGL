#pragma once

#include <stdexcept>

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
		detail::Events mEvents;
	public:
		bool mIsOpen;
		RECT mWinGlobalSize;
		int mWidth, mHeight;

		WinHandler(const int width, const int height, const std::wstring& title, Mouse& _VMouse);
		~WinHandler();

		void init_opengl();
		void enale_opengl_context() noexcept;
		void mouse_raw_input(bool _Mode = true);
		void show_cursor(bool mode);
		void swap_buffers() noexcept;
		void destroy() noexcept;
		detail::Events& events() noexcept;
	};

	#endif // __WIN32
}

