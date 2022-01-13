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

		bool WinKeyBoard::is_copy() const noexcept
		{
			return mKeyStates[VK_CONTROL] && mKeyStates['C'];
		}

		bool WinKeyBoard::is_default() const noexcept
		{
			return mKeyStates[VK_MENU] && mKeyStates[VK_CONTROL] && mKeyStates['D'];
		}

		bool WinKeyBoard::is_move_front() const noexcept
		{
			return mKeyStates['W'];
		}

		bool WinKeyBoard::is_move_back() const noexcept
		{
			return mKeyStates['S'];
		}

		bool WinKeyBoard::is_move_right() const noexcept
		{
			return mKeyStates['D'];
		}

		bool WinKeyBoard::is_move_left() const noexcept
		{
			return mKeyStates['A'];
		}



	}
#endif // _WIN32



}