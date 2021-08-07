#include <src/App/App.h>
#include <assert.h>


namespace tgl
{
	IUserData::IUserData()
	{}

	IUserData::~IUserData()
	{}

	void App::Init(int argn, char** argc)
	{

	}

	int App::Run(std::function<void(void)> _Fn)
	{
		#ifdef _WIN32
		win::MSG msg;
		size_t nextUpdate{};
		constexpr size_t fpsLock = 1000/60;

		while (true)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				win::TranslateMessage(&msg);
				win::DispatchMessage(&msg);
			}

			size_t ms = win::GetTickCount64();
			auto msNext = nextUpdate;
			size_t wait{};
			size_t ret{ WAIT_TIMEOUT };

			if (ms < msNext)
			{
				wait = min(fpsLock, msNext - ms);
				_Fn();
			}

			if (wait <= 1)
			{
				nextUpdate = ms + fpsLock;
				_Fn();
			}

			assert((wait & 0xffff0000) == 0);
			if (win::MsgWaitForMultipleObjects(0, nullptr, FALSE, (unsigned)wait, QS_ALLEVENTS) == WAIT_TIMEOUT)
			{
				nextUpdate = win::GetTickCount64() + fpsLock;
				_Fn();
			}

		}
		#endif
	}
}