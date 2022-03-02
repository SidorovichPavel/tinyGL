#include "KeyBoard.hpp"

namespace tgl
{
#ifdef _WIN32
	namespace win
	{
		#include <Windows.h>

		WinKeyBoard::WinKeyBoard()
		{}

		WinKeyBoard::~WinKeyBoard()
		{}

		void WinKeyBoard::key_down(uint64_t _KeyCode, int64_t _KeyState)
		{
			if (_KeyCode < 1024)
				mKeyStates[static_cast<size_t>(_KeyCode)] = true;
		}

		void WinKeyBoard::key_up(uint64_t _KeyCode, int64_t _KeyState)
		{
			if (_KeyCode < 1024)
				mKeyStates[static_cast<size_t>(_KeyCode)] = false;
		}

		detail::KeyBoardEvents& win::WinKeyBoard::events() noexcept
		{
			return mEvents;
		}

		bool WinKeyBoard::is_copy_combo() const noexcept
		{
			return mKeyStates[VK_CONTROL] && mKeyStates['C'];
		}

	}
#endif // _WIN32



}