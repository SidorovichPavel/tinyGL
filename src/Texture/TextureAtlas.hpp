#pragma once

#include "../../GL/GLFuncs.hpp"
#include <src/SOIL.h>
#include <cstdint>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>

namespace tgl
{
	class TextureAtlas2D
	{
		uint32_t mHandle;
		const int32_t mWidth;
		const int32_t mHeight;
	public:
		TextureAtlas2D();
		~TextureAtlas2D();

		void bind(uint32_t _Target = GL_TEXTURE_2D);
		static void unbind();

		template<int Height, int Width>
		void gen(const std::vector<std::string>& _Paths)
		{
			std::vector<uint8_t*> images(_Paths.size());
			int32_t width, height, chenel;
			for (auto i = 0; i < images.size(); ++i)
			{
				images[i] = SOIL_load_image(_Paths[i].c_str(), &width, &height, &chenel, SOIL_LOAD_RGB);
				if ((!images[i]) || (width != mWidth) || (height != mHeight))
					throw std::runtime_error("tinyGL exception, TextureAtlas2D, image loading failed");
			}

			auto atlasCount = Width * Height;
			auto finalWidth = Width * mWidth;
			auto finalHeight = Height * mHeight;

			if (static_cast<size_t>(atlasCount) < images.size())
				throw std::runtime_error("tinyGL exception, Texture2D, atlas size is too smal");

			for (; static_cast<size_t>(atlasCount) > images.size();)
				images.push_back(nullptr);

			std::vector<uint8_t> result(atlasCount * mWidth * mHeight * 3);
			std::fill(result.begin(), result.end(), 0);

			auto dest_pos = result.begin();
			for (auto i = 0; i < Height; ++i)
				for (auto offset = 0; offset < mHeight * mWidth * 3; offset += mWidth * 3)
					for (auto j = 0; j < Width; ++j)
					{
						if (images[i * Width + j])
						{
							std::copy_n(images[i * Width + j] + offset, mWidth * 3, dest_pos);
							dest_pos += mWidth * 3;
						}
						else
						{
							dest_pos += mWidth * 3;
							continue;
						}
					}

			#ifdef _DEBUG
			//SOIL_save_image("res/atlas.bmp", SOIL_SAVE_TYPE_BMP, finalWidth, finalHeight, 3, result.data());
			#endif

			bind();
			gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, finalWidth, finalHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, result.data());
			gl::GenerateMipmap(GL_TEXTURE_2D);
			for (auto& im : images)
				SOIL_free_image_data(im);
			unbind();
		}
	};

}