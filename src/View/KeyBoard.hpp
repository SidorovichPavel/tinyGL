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

			bool operator[](size_t _Idx) noexcept;
			uint8_t get_key_count(size_t _Idx) noexcept;
			void clear_key_count(size_t _Idx) noexcept;

			detail::KeyBoardEvents& events() noexcept;

			void key_down(uint64_t _KeyCode, int64_t _KeyState);
			void key_up(uint64_t _KeyCode, int64_t _KeyState);

			bool is_copy_combo() const noexcept;


		};
	}

	using KeyBoard = win::WinKeyBoard;

#endif
}