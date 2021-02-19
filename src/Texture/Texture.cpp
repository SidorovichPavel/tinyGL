#include <stdexcept>
#include <vector>
#include <iostream>
#include <src/Texture/Texture.h>
#include <src/SOIL.h>

namespace tgl
{

	Texture::Texture(const std::string& file_name)
		:
		mChenel(0)
	{
		auto path = "res/" + file_name;
		uint8_t* image = SOIL_load_image(path.c_str(), &mWidth, &mHeight, 0, SOIL_LOAD_RGB);

		if (!image)
			throw std::runtime_error("failed while load image");

		gl::glGenTextures(1, &mHandle);
		gl::glBindTexture(GL_TEXTURE_2D, mHandle);
		gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		gl::generateMipmap(GL_TEXTURE_2D);

		SOIL_free_image_data(image);
		gl::glBindTexture(GL_TEXTURE_2D, 0);
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