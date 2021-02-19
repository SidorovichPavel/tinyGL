#pragma once

#include <cstdint>
#include <string>
#include <GL/GLFuncs.h>

namespace tgl
{
	class Texture
	{
	public:
		Texture(const std::string& file_name);
		~Texture();

		void bind();
	private:
		uint32_t mHandle;
		int32_t mWidth;
		int32_t mHeight;
		int32_t mChenel;
	};




}