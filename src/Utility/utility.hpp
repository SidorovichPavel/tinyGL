#pragma once

#include <bitset>
#include <array>
#include <string>
#include <chrono>

namespace tgl
{
	namespace detail
	{
		struct FrameTimeInfo
		{
			std::chrono::milliseconds duration;
			std::chrono::time_point<std::chrono::steady_clock> timepoint;

			FrameTimeInfo() noexcept;
			FrameTimeInfo(std::chrono::milliseconds _ms, std::chrono::time_point<std::chrono::steady_clock> _TimePoint) noexcept;
			float s() noexcept;
			int64_t ms() noexcept;
		};

		void update_frame_time(FrameTimeInfo& _FrameTimeInfo) noexcept;

		template<class InType, class OutType, size_t Offset = 0>
		struct cutter
		{
			static_assert(sizeof(OutType) < sizeof(InType), "fail");
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