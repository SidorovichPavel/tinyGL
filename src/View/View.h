#pragma once

#include <string>
#include <src/style/style.h>

namespace tgl {

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	namespace win
	{
		#include <Windows.h>
		#include <windowsx.h>
		#include <WinUser.h>
	}

	class View
	{
		win::LRESULT WinProc(win::HWND, win::UINT, win::WPARAM, win::LPARAM) noexcept;
		static win::LRESULT CALLBACK SWinProc(win::HWND, win::UINT, win::WPARAM, win::LPARAM) noexcept;
		
		void init_opengl();

	protected:
		bool mIsOpen;
		win::HWND mHandle;
		win::HDC mDevice_context;
		win::HGLRC mGL_resource_content;
		win::RECT mWinSize;
	protected:
		int mWidth, mHeight;
		virtual void create();
		virtual void destroy();
		virtual void size(int width, int height);
		virtual void key_down(__int64 virtual_code, __int64 state);
		virtual void mouse_move(int x, int y, __int64 virtual_code);
		virtual void moving(win::RECT* pRect);
	public:
		View(const int width, const int height, const std::wstring& title, const Style& style = Style());
		View(const View& _Right) = delete;
		View(View&& _Right) noexcept;
		~View();
		
		win::HWND get_handle() noexcept;
		bool is_open() noexcept;
		void swap_buffers() noexcept;
		void enale_opengl_context() noexcept;

	};



#endif



}



