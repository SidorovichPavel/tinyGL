#pragma once

#include <string>

#include "../style/style.h"


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
		win::HWND handle;
		win::HDC device_content;
	protected:
		void create();
		void destroy();
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



