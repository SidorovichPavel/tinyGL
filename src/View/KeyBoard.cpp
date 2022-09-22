#include "KeyBoard.hpp"

#include <cassert>

namespace tgl
{
#ifdef _WIN32
	namespace win
	{

		WinKeyBoard::WinKeyBoard()
			:
			mKeyCounters(key_number)
		{}

		WinKeyBoard::~WinKeyBoard()
		{}

		bool WinKeyBoard::operator[](KeyCode _Code) noexcept
		{
			assert(static_cast<size_t>(_Code) < key_number);

			return mKeyStates[static_cast<size_t>(_Code)];
		}

		uint8_t WinKeyBoard::get_key_count(size_t _Idx) noexcept
		{
			assert(_Idx < key_number);

			return mKeyCounters[_Idx];
		}

		void WinKeyBoard::clear_key_count(size_t _Idx) noexcept
		{
			assert(_Idx < key_number);

			mKeyCounters[static_cast<size_t>(_Idx)] = 0;
		}

		void WinKeyBoard::key_down(uint64_t _KeyCode, int64_t _KeyState)
		{
			assert(_KeyCode < key_number);

			mKeyStates[static_cast<size_t>(_KeyCode)] = true;
		}

		void WinKeyBoard::key_up(uint64_t _KeyCode, int64_t _KeyState)
		{
			auto kc = static_cast<size_t>(_KeyCode);

			assert(_KeyCode < key_number);

			if (mKeyStates[kc])
				mKeyCounters[kc]++;
			mKeyStates[kc] = false;

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