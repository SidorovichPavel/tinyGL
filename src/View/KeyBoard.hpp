#pragma once

#include <bitset>

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

		class WinKeyBoard
		{
		private:
			detail::KeyBoardEvents mEvents;

		protected:
			std::bitset<1024> mKeyStates;

		public:
			WinKeyBoard();
			virtual ~WinKeyBoard();

			void key_down(uint64_t _KeyCode, int64_t _KeyState);
			void key_up(uint64_t _KeyCode, int64_t _KeyState);

			detail::KeyBoardEvents& events() noexcept;

			bool is_copy_combo() const noexcept;
		};
	}

	using KeyBoard = win::WinKeyBoard;

#endif
}