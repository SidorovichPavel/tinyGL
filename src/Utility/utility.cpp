#include "utility.hpp"

namespace tgl::detail
{
	FrameTimeInfo::FrameTimeInfo() noexcept
		:
		timepoint(std::chrono::steady_clock::now()),
		duration(std::chrono::milliseconds(10))
	{}

	FrameTimeInfo::FrameTimeInfo(std::chrono::milliseconds _ms, std::chrono::time_point<std::chrono::steady_clock> _TimePoint) noexcept
		:
		duration(_ms),
		timepoint(_TimePoint)
	{}

	float FrameTimeInfo::s() noexcept
	{
		return duration.count() / 1000.f;
	}

	int64_t FrameTimeInfo::ms() noexcept
	{
		return duration.count();
	}

	void update_frame_time(FrameTimeInfo& _FrameTimeInfo) noexcept
	{
		auto current = std::chrono::steady_clock::now();
		_FrameTimeInfo.duration = std::chrono::duration_cast<std::chrono::milliseconds>(current - _FrameTimeInfo.timepoint);
		_FrameTimeInfo.timepoint = current;
	}
}