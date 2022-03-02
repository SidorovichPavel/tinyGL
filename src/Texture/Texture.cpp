#include <stdexcept>
#include <vector>
#include <iostream>
#include "Texture.hpp"
#include <SOIL.h>

namespace tgl
{
	Texture2D::Texture2D(const std::string& file_name)
	{
		uint8_t* image = SOIL_load_image(file_name.c_str(), &mWidth, &mHeight, &mChanels, SOIL_LOAD_RGB);

		if (!image)
			throw std::runtime_error("failed while load image");
		
		gl::glGenTextures(1, &mHandle);
		gl::glBindTexture(GL_TEXTURE_2D, mHandle);
		gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		gl::GenerateMipmap(GL_TEXTURE_2D);
		gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		SOIL_free_image_data(image);
		gl::glBindTexture(GL_TEXTURE_2D, 0);
	}

	template<>
	Texture2D::Texture2D(const uint8_t* const _Data, const int _Width, const int _Height) noexcept
		:
		mWidth(_Width),
		mHeight(_Height),
		mChanels(0)
	{
		gl::glGenTextures(1, &mHandle);
		gl::glBindTexture(GL_TEXTURE_2D, mHandle);
		gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, _Data);
		gl::GenerateMipmap(GL_TEXTURE_2D);
		gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	template<>
	Texture2D::Texture2D(const float* const _Data, const int _Width, const int _Height) noexcept
		:
		mWidth(_Width),
		mHeight(_Height),
		mChanels(0)
	{
		gl::glGenTextures(1, &mHandle);
		gl::glBindTexture(GL_TEXTURE_2D, mHandle);
		gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_FLOAT, _Data);
		gl::GenerateMipmap(GL_TEXTURE_2D);
		gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	Texture2D::~Texture2D()
	{
		gl::glDeleteTextures(1, &mHandle);
	}

	Texture2D::Texture2D(Texture2D&& _Other) noexcept
		:
		mHandle(0),
		mWidth(0),
		mHeight(0),
		mChanels(0)
	{
		_swap(_Other);
	}

	Texture2D& Texture2D::operator=(Texture2D&& _Right) noexcept
	{
		_swap(_Right);
		return *this;
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
		bind();
		gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, _Data);
		gl::GenerateMipmap(GL_TEXTURE_2D);
		unbind();
	}

	void Texture2D::_swap(Texture2D& _Other) noexcept
	{
		std::swap(this->mHandle, _Other.mHandle);
		std::swap(this->mWidth, _Other.mWidth);
		std::swap(this->mHeight, _Other.mHeight);
		std::swap(this->mChanels, _Other.mChanels);
	}






}