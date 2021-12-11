#include <stdexcept>
#include <vector>
#include <iostream>
#include "Texture.h"
#include <src/SOIL.h>

namespace tgl
{
	Texture2D::Texture2D(const std::string& file_name)
	{
		auto path = "res/" + file_name;
		uint8_t* image = SOIL_load_image(path.c_str(), &mWidth, &mHeight, &mChanels, SOIL_LOAD_RGB);

		if (!image)
			throw std::runtime_error("failed while load image");

		gl::glGenTextures(1, &mHandle);
		gl::glBindTexture(GL_TEXTURE_2D, mHandle);
		gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		gl::GenerateMipmap(GL_TEXTURE_2D);

		SOIL_free_image_data(image);
		gl::glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture2D::Texture2D(const uint8_t* const _Data, const int _Width, const int _Height) noexcept
		:
		mWidth(_Width),
		mHeight(_Height),
		mChanels(0)
	{
		gl::glGenTextures(1, &mHandle);
		bind();
		gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, _Data);
		gl::GenerateMipmap(GL_TEXTURE_2D);

		unbind();
	}

	void Texture2D::bind(int32_t _Target)
	{
		gl::glBindTexture(_Target, mHandle);
	}

	void Texture2D::unbind(int32_t _Target)
	{
		gl::glBindTexture(_Target, 0);
	}

	void Texture2D::update(const uint8_t* const _Data)
	{
		gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, _Data);
		gl::GenerateMipmap(GL_TEXTURE_2D);

		gl::glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture2D::Texture2D(Texture2D&& _Other) noexcept
		:
		mHandle(0),
		mWidth(0),
		mHeight(0),
		mChanels(0)
	{
		std::swap(this->mHandle, _Other.mHandle);
		std::swap(this->mWidth, _Other.mWidth);
		std::swap(this->mHeight, _Other.mHeight);
		std::swap(this->mChanels, _Other.mChanels);
	}

	Texture2D::~Texture2D()
	{
		gl::glDeleteTextures(1, &mHandle);
	}

	Texture2D& Texture2D::operator=(Texture2D&& _Right) noexcept
	{
		std::swap(this->mHandle, _Right.mHandle);
		std::swap(this->mWidth, _Right.mWidth);
		std::swap(this->mHeight, _Right.mHeight);
		return *this;
	}

	
	

}