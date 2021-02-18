#pragma once

#include <string>
#include <src/style/style.h>
#include <src/Event/Event.h>
#include <src/View/Mouse.h>

namespace tgl {

	#ifdef _WIN32
	namespace win
	{
		#define WIN32_LEAN_AND_MEAN
		#include <Windows.h>
		#include <windowsx.h>
		#include <WinUser.h>

		constexpr unsigned MSG_GETRIBUFFER = WM_USER + 1;
	}

	class View final
	{
		win::HWND mHandle;
		win::HDC mDevice_context;
		win::HGLRC mGL_resource_content;
		win::LRESULT WinProc(win::HWND, win::UINT, win::WPARAM, win::LPARAM) noexcept;
		static win::LRESULT CALLBACK SWinProc(win::HWND, win::UINT, win::WPARAM, win::LPARAM) noexcept;

		bool mIsOpen;
		bool mMouseRawInput;
		
		Mouse mVirtualMouse;
	public:
		Event<void()> create_event;
		/*desc
		* x, y
		*/
		Event<void(uint16_t, uint16_t)> size_event;
		/*desc
		* first - The virtual - key code of the nonsystem key.See Virtual - Key Codes.
		* second - The repeat count, scan code,
		* extended-key flag, context code, previous key-state flag,
		* and transition-state flag, as shown following.
		*/
		Event<void(int64_t, int64_t)> key_down_event;
		/*desc
		* first - The virtual-key code of the nonsystem key. See Virtual-Key Codes.
		* second - The repeat count, scan code, extended-key flag, context code,
		* previous key-state flag, and transition-state flag, as shown in the following table.
		*/
		Event<void(int64_t, int64_t)> key_up_event;
		/*desc
		* first - x
		* second - y
		* third - key state... read MSDN
		*/
		Event<void(int32_t, int32_t, int64_t)> mouse_move_event;
		/*desc
		* first - key state
		* second - distance the wheel is rotated,
		* expressed in multiples or divisions of WHEEL_DELTA, which is 120.
		* A positive value indicates that the wheel was rotated forward, away from the user;
		* a negative value indicates that the wheel was rotated backward, toward the user.
		*/
		Event<void(uint16_t, int16_t, int32_t, int32_t)> mouse_wheel_event;
		/*
		* The xand y coordinates of the upper - left corner of the client area of the window.
		*/
		Event<void(int32_t, int32_t)>move_event;

		Event<void(void)> raw_input_event;

		Event<void(win::RECT*)> moving_event;

		Event<void(int32_t, int32_t)> mouse_raw_input_event;

		int mWidth, mHeight;
		win::RECT mWinGlobalSize;

		View(const int width, const int height, const std::wstring& title, const Style& style = Style());
		View(const View& _Right) = delete;
		View(View&& _Right) noexcept;
		~View();

		win::HWND get_handle() noexcept;
		void enable_mouse_raw_input();
		void disable_mouse_raw_input();
		void set_title(const std::wstring& title);
		void show_cursor(bool mode);
		void center_cursour();
		std::pair<int, int> get_global_center();
		bool is_open() noexcept;
		void swap_buffers() noexcept;
		void enale_opengl_context() noexcept;
		void destroy() noexcept;

	private:
		void init_opengl();
	};



	#endif



}



