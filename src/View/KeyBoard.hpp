#pragma once

#include <bitset>

#include "..\Event\Event.hpp"

namespace tgl
{
	#ifdef _WIN32
	namespace win
	{
		#include <Windows.h>

		enum class KeyCode : uint32_t
		{
			A = 'A',
			B = 'B',
			C = 'C',
			D = 'D',
			E = 'E',
			S = 'S',
			W = 'W',
			UP = VK_UP,
			LEFT = VK_LEFT,
			DOWN = VK_DOWN,
			RIGHT = VK_RIGHT,
			TAB = VK_TAB,
			ALT = VK_MENU,
			RALT = VK_RMENU,
			CTRL = VK_CONTROL,
			RCTRL = VK_RCONTROL,
		};

		class WinKeyBoard
		{
		public:
			WinKeyBoard();
			virtual ~WinKeyBoard();

			bool operator[](KeyCode _Code) noexcept;

			void key_down(uint64_t _KeyCode, int64_t _KeyState);
			void key_up(uint64_t _KeyCode, int64_t _KeyState);

		protected:
			std::bitset<1024> mKeyStates;
		};
	}

	using KeyBoard = win::WinKeyBoard;
	using KeyCode = win::KeyCode;
	#endif
}