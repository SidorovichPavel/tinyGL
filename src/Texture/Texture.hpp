#pragma once

#include <cstdint>
#include <string>
#include "../../GL/GLFuncs.hpp"

namespace tgl
{
	class Texture2D
	{
	public:
		Texture2D(const std::string& file_name);
		template<class T>
		Texture2D(const T* const _Data, const int _Width, const int _Height) noexcept
		{
			if constexpr (std::is_arithmetic_v<T>)
				static_assert(false, "sorry, i don't added implementation for this type");
			else
				static_assert(false, "HM....");
		}
		~Texture2D();
		Texture2D(const Texture2D&) = delete;
		Texture2D operator=(const Texture2D&) = delete;
		Texture2D(Texture2D&& _Other) noexcept;
		Texture2D& operator=(Texture2D&& _Right) noexcept;

		void bind(int32_t _Target = GL_TEXTURE_2D);
		void unbind(int32_t _Target = GL_TEXTURE_2D);

		void update(const uint8_t* const _Data);
	private:
		uint32_t mHandle;
		int32_t mWidth;
		int32_t mHeight;
		int32_t mChanels;

		void _swap(Texture2D& _Other) noexcept;
	};
}