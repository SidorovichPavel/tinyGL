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
		Texture2D(const T* _Data, int _Width, int _Height) noexcept
		{
			static_assert(std::is_arithmetic_v<T>, "sorry, i don't added implementation for this type");
			
			//static_assert(false, "HM....");
		}
		template<>
		Texture2D(const uint8_t* _Data, int _Width, int _Height) noexcept;
		template<>
		Texture2D(const float* _Data, int _Width, int _Height) noexcept;

		/* only for float data */
		Texture2D(nullptr_t _Data, int _Width, int _Height);

		~Texture2D();
		Texture2D(const Texture2D&) = delete;
		Texture2D operator=(const Texture2D&) = delete;
		Texture2D(Texture2D&& _Other) noexcept;
		Texture2D& operator=(Texture2D&& _Right) noexcept;

		void bind(int32_t _Target = GL_TEXTURE_2D);
		void unbind(int32_t _Target = GL_TEXTURE_2D);

		uint32_t get_handle() noexcept;
	private:
		uint32_t mHandle;
		int32_t mWidth;
		int32_t mHeight;
		int32_t mChanels;

		void _swap(Texture2D& _Other) noexcept;
	};
}