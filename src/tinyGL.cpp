#include "tinyGL.h"

namespace tgl
{
	void event_pool() noexcept
	{
	#ifdef _WIN32
		win::MSG msg;
		while (win::PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE))
		{
			win::GetMessage(&msg, nullptr, 0, 0);
			win::TranslateMessage(&msg);
			win::DispatchMessage(&msg);
		}
	#else
		//TODO
	#endif
	}
	

}