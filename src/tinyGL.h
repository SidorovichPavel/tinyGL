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
	void event_pool() noexcept;
	void Init();
}

