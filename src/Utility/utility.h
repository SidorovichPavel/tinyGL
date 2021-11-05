#pragma once

#include <bitset>
#include <array>

namespace tgl
{
	namespace detail
	{
		template<class InType, class OutType, bool Offset = false>
		struct cutter
		{
			static OutType& get(InType& _In) noexcept
			{
				if constexpr (Offset)
				{
					return *(reinterpret_cast<OutType*>(&_In) + 1);
				}
				else
				{
					return *(reinterpret_cast<OutType*>(&_In) + 0);
				}
			}

			static OutType get(InType&& _In) noexcept
			{
				if constexpr (Offset)
				{
					return *(reinterpret_cast<OutType*>(&_In) + 1);
				}
				else
				{
					return *(reinterpret_cast<OutType*>(&_In) + 0);
				}
			}
		};
	}

	using hi_word = detail::cutter<unsigned int, unsigned short int, true>;
	using lo_word = detail::cutter<unsigned int, unsigned short int>;
	using get_x = lo_word;
	using get_y = hi_word;


}