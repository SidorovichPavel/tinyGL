#pragma once

#include <string>
#include <src/style/style.h>

namespace tgl {

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	namespace win
	{
		#include <Windows.h>
	}

	class View
	{
		win::LRESULT WinProc(win::HWND, win::UINT, win::WPARAM, win::LPARAM) noexcept;
		static win::LRESULT CALLBACK SWinProc(win::HWND, win::UINT, win::WPARAM, win::LPARAM) noexcept;
		
		void init_opengl();
		
		bool isOpen;
		win::HWND mHandle;
		win::HDC mDevice_content;
		win::HGLRC mGL_resource_content;

		int mWidth, mHeight;
	protected:
		virtual void create();
		virtual void destroy();
		virtual void size(int width, int height);
		virtual void key_down(__int64 virtual_code, __int64 state);
	public:
		View(const int width, const int height, const std::wstring& title, const Style& style = Style());
		View(const View& _Right) = delete;
		View(View&& _Right) noexcept;
		~View();
		
		bool is_open() noexcept;
		void swap_buffers() noexcept;
	};



#endif



}



