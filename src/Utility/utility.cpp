#include "utility.hpp"

namespace tgl::detail
{
	FrameTimeInfo update_frame_time(std::chrono::time_point<std::chrono::steady_clock> _Prev_TP)
	{
		auto current = std::chrono::steady_clock::now();
		std::chrono::duration<float> duration = current - _Prev_TP;
		
		return FrameTimeInfo(duration.count(), current);
	}

}