#pragma once

#include <cstdint>
#include <string>
#include <GL/GLFuncs.h>

namespace tgl
{
	class Texture2D
	{
	public:
		Texture2D(const std::string& file_name);
		~Texture2D();

		void bind(int32_t _Target = GL_TEXTURE_2D);
	private:
		uint32_t mHandle;
		int32_t mWidth;
		int32_t mHeight;
	};




}