#pragma once

#include "../Event/Event.hpp"

namespace tgl
{
#ifdef _WIN32
	namespace win
	{
		#include <Windows.h>
	}

	class Timer
	{
		win::UINT_PTR mHandle;
		static void CALLBACK STimerProc(win::HWND _Wnd, win::UINT _Msg, win::UINT_PTR _Handle, win::DWORD _Ms);
	public:
		Timer(win::HWND _Win, unsigned _Delay);

		Timer(const Timer&&) = delete;
		Timer& operator=(const Timer&) = delete;
		
		Timer(Timer&& _Other) noexcept;
		Timer& operator=(Timer&& _Right) noexcept;

		Event<void(void)> booom;
		~Timer();
	};
#endif

}
