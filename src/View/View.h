#pragma once

#include <string>
#include <src/style/style.h>
#include <src/Event/Event.h>

namespace tgl {

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	namespace win
	{
		#include <Windows.h>
		#include <windowsx.h>
		#include <WinUser.h>
	}

	class View final
	{
		win::LRESULT WinProc(win::HWND, win::UINT, win::WPARAM, win::LPARAM) noexcept;
		static win::LRESULT CALLBACK SWinProc(win::HWND, win::UINT, win::WPARAM, win::LPARAM) noexcept;
		
		void init_opengl();

		bool mIsOpen;
		win::HWND mHandle;
		win::HDC mDevice_context;
		win::HGLRC mGL_resource_content;
		win::RECT mWinSize;
	
		int mWidth, mHeight;
	public:
		Event<void()> create_event;
		Event<void()> destroy_event;
		Event<void(uint16_t, uint16_t)> size_event;
		Event<void(__int64, __int64)> key_down_event;
		Event<void(int, int, __int64)> mouse_move_event;
		Event<void(win::RECT*)> moving_event;
		
		View(const int width, const int height, const std::wstring& title, const Style& style = Style());
		View(const View& _Right) = delete;
		View(View&& _Right) noexcept;
		~View();
		
		win::HWND get_handle() noexcept;
		void set_title(const std::wstring& title);
		bool is_open() noexcept;
		void swap_buffers() noexcept;
		void enale_opengl_context() noexcept;
	};



#endif



}



