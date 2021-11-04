#pragma once

#include <GL/GLFuncs.h>

namespace tgl
{
	extern std::atomic<bool> opengl_is_init;

	std::pair<bool, int> event_pool() noexcept;
	void callback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, char const* message, void const* user_param);
	void Init();
}

