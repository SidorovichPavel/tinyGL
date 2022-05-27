#include "KeyBoard.hpp"

namespace tgl
{
#ifdef _WIN32
	namespace win
	{
		WinKeyBoard::WinKeyBoard()
		{}

		WinKeyBoard::~WinKeyBoard()
		{}

		bool WinKeyBoard::operator[](KeyCode _Code) noexcept
		{
			return mKeyStates[static_cast<uint32_t>(_Code)];
		}

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


	}
#endif // _WIN32



}