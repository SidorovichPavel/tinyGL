#include <src/Timer/Timer.h>


#pragma warning(disable:26495)

namespace tgl
{
	void Timer::STimerProc(win::HWND _Wnd, win::UINT _Msg, win::UINT_PTR _Handle, win::DWORD _Ms)
	{
		auto pTimer = reinterpret_cast<Timer*>(_Handle);
		(*pTimer).booom();
	}

	Timer::Timer(win::HWND _Win, unsigned _Delay) :
		mHandle(reinterpret_cast<win::UINT_PTR>(this))
	{
		win::SetTimer(_Win, mHandle, _Delay, Timer::STimerProc);
	}

	Timer::Timer(Timer&& _Other) noexcept
	{
		this->mHandle = _Other.mHandle;
		//booom.swap(&_Other.booom);
	}

	Timer& Timer::operator=(Timer&& _Right) noexcept
	{
		this->mHandle = _Right.mHandle;
		//booom.swap(&_Right.booom);
		return *this;
	}

	Timer::~Timer()
	{
		win::KillTimer(0, mHandle);
	}
}