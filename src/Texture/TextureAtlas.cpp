#include <src/Texture/TextureAtlas.h>
#include <src/SOIL.h>
#include <iterator>
#include <algorithm>

namespace tgl
{

	TextureAtlas2D::TextureAtlas2D()
		:
		mWidth(1024),
		mHeight(1024)
	{
		gl::glGenTextures(1, &mHandle);
	}

	TextureAtlas2D::~TextureAtlas2D()
	{
		gl::glDeleteTextures(1, &mHandle);
	}

	void TextureAtlas2D::bind(uint32_t _Target)
	{
		gl::glBindTexture(_Target, mHandle);
	}

	void TextureAtlas2D::unbind()
	{
		tgl::gl::glBindTexture(GL_TEXTURE_2D, 0);
	}

}