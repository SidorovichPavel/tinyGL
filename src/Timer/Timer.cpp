#include "Timer.h"

namespace tgl
{

	void Timer::STimerProc(win::HWND _Wnd, win::UINT _Msg, win::UINT_PTR _Handle, win::DWORD _Ms)
	{
		auto pTimer = reinterpret_cast<Timer*>(_Handle);
		(*pTimer)();
	}

	Timer::Timer(win::HWND _Win, unsigned _Delay) :
		mCheck(false),
		mHandle(reinterpret_cast<win::UINT_PTR>(this))
	{
		win::SetTimer(_Win, mHandle, _Delay, Timer::STimerProc);
	}

	void Timer::operator()()
	{
		mCheck = true;
	}

	bool Timer::check()
	{
		return mCheck;
	}

	void Timer::reset()
	{
		mCheck = false;
	}

	Timer::~Timer()
	{
		win::KillTimer(0, mHandle);
	}
}