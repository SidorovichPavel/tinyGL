#pragma once

#include <bitset>
#include <array>
#include <string>

namespace tgl
{
	namespace detail
	{
		template<class InType, class OutType, size_t Offset = 0>
		struct cutter
		{
			static_assert(sizeof(InType) >= sizeof(OutType), "fail");
			constexpr static OutType& get(InType& _In) noexcept
			{
				return *(reinterpret_cast<OutType*>(&_In) + Offset);
			}

			constexpr static OutType get(InType&& _In) noexcept
			{
				return *(reinterpret_cast<OutType*>(&_In) + Offset);
			}
		};
	}

	using hi_word = detail::cutter<unsigned int, unsigned short int, 1>;
	using lo_word = detail::cutter<unsigned int, unsigned short int>;
	using cut_y = hi_word;
	using cut_x = lo_word;
	using cut_key_state = detail::cutter<unsigned long long, unsigned short>;
	using cut_wheel_delta = detail::cutter<unsigned long long, unsigned short, 1>;

#ifdef UNICODE
	using string = std::wstring;
#else
	using string = std::string;
#endif
}