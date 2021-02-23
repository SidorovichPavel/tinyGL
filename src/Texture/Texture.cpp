#include <stdexcept>
#include <vector>
#include <iostream>
#include <src/Texture/Texture.h>
#include <src/SOIL.h>

namespace tgl
{

	Texture2D::Texture2D(const std::string& file_name)
	{
		auto path = "res/" + file_name;
		uint8_t* image = SOIL_load_image(path.c_str(), &mWidth, &mHeight, 0, SOIL_LOAD_RGB);

		if (!image)
			throw std::runtime_error("failed while load image");

		gl::glGenTextures(1, &mHandle);
		gl::glBindTexture(GL_TEXTURE_2D, mHandle);
		gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		gl::GenerateMipmap(GL_TEXTURE_2D);

		SOIL_free_image_data(image);
		gl::glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture2D::~Texture2D()
	{
		gl::glDeleteTextures(1, &mHandle);
	}

	void Texture2D::bind(int32_t _Target)
	{
		gl::glBindTexture(_Target, mHandle);
	}

}