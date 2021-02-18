#pragma once

#include <src/View/View.h>
#include <GL/GLFuncs.h>
#include <GL/VAO/VAO.h>
#include <GL/Shader/Shader.h>
#include <GL/Texture/Texture.h>
#include <src/Timer/Timer.h>
#include <src/App/App.h>

namespace tgl
{
	void event_pool() noexcept;
	void Init();
}

