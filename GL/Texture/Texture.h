#pragma once

#include <cstdint>
#include <vector>
#include <GL/GLFuncs.h>

namespace tgl
{
	class Texture
	{
	public:
		Texture(const uint32_t _Width, const uint32_t _Height, std::vector<uint8_t>&& data);
		~Texture();

		void bind();
	private:
		uint32_t mHandle;
		uint32_t mWidth;
		uint32_t mHeight;
	};




}