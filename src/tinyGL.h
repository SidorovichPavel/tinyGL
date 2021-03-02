#pragma once

#include <src/View/View.h>
#include <src/Mesh/Mesh.h>
#include <src/Shader/Shader.h>
#include <src/Texture/Texture.h>
#include <src/Texture/TextureAtlas.h>
#include <src/Timer/Timer.h>
#include <src/App/App.h>
#include <GL/GLFuncs.h>

namespace tgl
{
	extern std::atomic<bool> opengl_is_init;

	void event_pool() noexcept;
	void callback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, char const* message, void const* user_param);
	void Init();
}

