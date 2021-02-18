#include <GL/Texture/Texture.h>

namespace tgl
{

	Texture::Texture(const uint32_t _Width, const uint32_t _Height, std::vector<uint8_t>&& data)
		:
		mWidth(_Width),
		mHeight(_Height)
	{
		gl::glGenTextures(1, &mHandle);
		gl::glBindTexture(GL_TEXTURE_2D, mHandle);
		gl::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, 1u, GL_UNSIGNED_BYTE, data.data());
		gl::glTexParameteri(mHandle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		gl::glTexParameteri(mHandle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	Texture::~Texture()
	{
		gl::glDeleteTextures(1, &mHandle);
	}

	void Texture::bind()
	{
		gl::glBindTexture(GL_TEXTURE_2D, mHandle);
	}

}