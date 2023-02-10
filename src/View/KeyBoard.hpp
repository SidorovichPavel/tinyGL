#pragma once

#include <bitset>
#include <vector>

#include "..\Event\Event.hpp"

namespace tgl
{
#ifdef _WIN32
	namespace win
	{
		namespace detail
		{
			struct KeyBoardEvents
			{
				Event<void()> copy;
				Event<void()> cutting;
				Event<void()> save;
			};
		}

#include <Windows.h>
		enum class KeyCode
		{
			W = 'W',
			A = 'A',
			S = 'S',
			D = 'D',
			Up = VK_UP,
			Left = VK_LEFT,
			Down = VK_DOWN,
			Right = VK_RIGHT,
			TAB = VK_TAB,
			Shift = VK_SHIFT,
			Escape = VK_ESCAPE,
			Return = VK_RETURN,
			Back = VK_BACK,
			Space = VK_SPACE
		};

		class WinKeyBoard
		{
		private:
			detail::KeyBoardEvents mEvents;

		protected:
			static constexpr size_t key_number = 1024;
			std::bitset<key_number> mKeyStates;
			std::vector<uint8_t> mKeyCounters;

		public:
			WinKeyBoard();
			virtual ~WinKeyBoard();

			bool operator[](KeyCode _Code) noexcept;
			uint8_t get_key_count(size_t _Idx) noexcept;
			void clear_key_count(size_t _Idx) noexcept;

			detail::KeyBoardEvents& events() noexcept;

			void key_down(uint64_t _KeyCode, int64_t _KeyState);
			void key_up(uint64_t _KeyCode, int64_t _KeyState);

			bool is_copy_combo() const noexcept;


		};
	}

	using KeyBoard = win::WinKeyBoard;
	using KeyCode = win::KeyCode;
#endif
}