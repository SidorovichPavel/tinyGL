#include "utility.hpp"

namespace tgl::detail
{
	using namespace std::chrono_literals;
	FrameTimeInfo::FrameTimeInfo() noexcept
		:
		timepoint(std::chrono::steady_clock::now()),
		duration(16ms),
		next_update(0ms)
	{}

	FrameTimeInfo::FrameTimeInfo(std::chrono::milliseconds _ms, std::chrono::time_point<std::chrono::steady_clock> _TimePoint) noexcept
		:
		timepoint(_TimePoint),
		duration(_ms),
		next_update(0ms)
	{}

	float FrameTimeInfo::s() noexcept
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / 1000.f;
	}

	int64_t FrameTimeInfo::ms() noexcept
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	}

	void FrameTimeInfo::update_frame_time() noexcept
	{
		auto current = std::chrono::steady_clock::now();
		duration = std::chrono::duration_cast<std::chrono::milliseconds>(current - timepoint);
		timepoint = current;
	}

}