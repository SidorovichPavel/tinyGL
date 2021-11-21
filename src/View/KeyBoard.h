#pragma once

#include <bitset>
//#include <>

namespace tgl
{
	namespace detail
	{

	}
#ifdef _WIN32
	namespace win
	{
		class WinKeyBoard
		{
			std::bitset<1024> mKeyStates;

		public:
			WinKeyBoard();
			~WinKeyBoard();

			void key_down(uint64_t _KeyCode, int64_t _KeyState);
			void key_up(uint64_t _KeyCode, int64_t _KeyState);

			bool is_copy() const noexcept;
			bool is_default() const noexcept;

			bool is_move_front() const noexcept;
			bool is_move_back() const noexcept;
			bool is_move_right() const noexcept;
			bool is_move_left() const noexcept;
		};
	}

	using KeyBoard = win::WinKeyBoard;
#endif
}