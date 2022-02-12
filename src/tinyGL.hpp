#pragma once

#include "..\GL\GLFuncs.hpp"

namespace tgl
{
	extern std::atomic<bool> opengl_is_init;
	void Init();

	std::pair<bool, int> event_pool() noexcept;

	void clear_black() noexcept;
	/*clamp(0.f,1.f,params)*/
	void clear_color(float _R, float _G, float _B) noexcept;
	void view_port(int16_t _Width, int16_t _Height);
	void view_port_ex(int16_t _X, int16_t _Y, int16_t _Width, int16_t _Height);

	namespace gl
	{
		void _stdcall callback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, 
			int32_t length, char const* message, void const* user_param);
	}
}

