#pragma once

#include <memory>

#include "../Utility/utility.hpp"
#include "../Event/Event.hpp"
#include "Style.hpp"
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

		enum class DrawLib
		{
			Kernel32,
			OpenGL
		};

		class WinHandler
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

			UINT mRawInputSize;
			HRAWINPUT mRawInputHandle;
			std::vector<uint8_t> mRawInputData;

			detail::Events mEvents;
			
			static LRESULT CALLBACK GeneralProc(HWND, UINT, WPARAM, LPARAM) noexcept;
			LRESULT WinProc(HWND, UINT, WPARAM, LPARAM) noexcept;
			void mouse_raw_input(bool _Mode);
			DrawLib MDrawLib;

		protected:
			bool mIsOpen;
			int mWidth, mHeight;
			int32_t mScreenWidth, mScreenHeight;
			
		public:
			WinHandler(const Style* _Style_Ptr);
			virtual ~WinHandler();
			
			detail::Events& get_events() noexcept;

			void init_opengl();
			void enable_opengl_context() noexcept;
			void swap_buffers() noexcept;
			void show_cursor(bool _Mode) noexcept;
			void set_title(const std::string& title) noexcept;

			virtual std::pair<int32_t, int32_t> get_size() noexcept = 0;
			virtual float get_ratio() noexcept = 0;
			virtual bool is_open() noexcept = 0;
		protected:
			void invalidate_rect() noexcept;
			void send_message(uint32_t _Msg, uint64_t _WParam, int64_t _LParam) noexcept;
			void post_message(uint32_t _Msg, uint64_t _WParam, int64_t _LParam) noexcept;
			void close() noexcept;
			void destroy() noexcept;
		};
	}
	using Handler = win::WinHandler;
#endif // __WIN32

}

