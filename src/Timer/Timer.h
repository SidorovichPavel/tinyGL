#pragma once

#include <src/Event/Event.h>

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
		bool mCheck;
		static void CALLBACK STimerProc(win::HWND _Wnd, win::UINT _Msg, win::UINT_PTR _Handle, win::DWORD _Ms);
	public:
		Timer(win::HWND _Win, unsigned _Delay);
		Event<void(void)> booom;
		bool check();
		void reset();
		~Timer();
	};
#endif

}
