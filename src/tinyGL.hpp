#pragma once

#include <thread>
#include <queue>
#include <functional>
#include <future>

#include "..\GL\GLFuncs.hpp"

#include "View\View.hpp"
#include "Utility\utility.hpp"

namespace tgl
{
	extern std::atomic<bool> opengl_is_init;

	void Init();

	extern detail::FrameTimeInfo FrameTimeInfo;

	std::pair<bool, int> event_pool(int fps, detail::FrameTimeInfo& fti = tgl::FrameTimeInfo) noexcept;

	void clear_black() noexcept;
	/*clamp(0.f,1.f,each param)*/
	void clear_color(float _R, float _G, float _B) noexcept;
	void view_port(uint16_t _Width, uint16_t _Height);
	void view_port_ex(int16_t _X, int16_t _Y, int16_t _Width, int16_t _Height);

	namespace gl
	{
		void _stdcall callback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, 
			int32_t length, char const* message, void const* user_param);
	}
}

