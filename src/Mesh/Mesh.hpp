#pragma once

#include <vector>
#include <array>
#include "../../GL/GLFuncs.hpp"

namespace tgl
{
	namespace detail
	{
		template<size_t... Args>
		struct unpack_sequence
		{
			constexpr static size_t Elems[sizeof...(Args)] = { Args... };
			constexpr static size_t get_sum()
			{
				size_t res = 0;
				for (auto i = 0; i < sizeof...(Args); ++i)
					res += Elems[i];
				return res;
			}
			constexpr static size_t get(size_t _Index)
			{
				return Elems[_Index];
			}
			constexpr static size_t count = sizeof...(Args);
		};
	}

	class Mesh
	{
		uint32_t	mVAO;
		uint32_t	mIndicesBuffer;
		int32_t		mIndicesCount;
		uint32_t	mBuffer;
		size_t		mVertexSize;
	public:
		Mesh();
		~Mesh();
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh(Mesh&& _Other) noexcept;
		Mesh& operator=(Mesh&& _Right) noexcept;

		void set_indices(size_t _Count, uint32_t* _Elems);
		void draw(uint32_t _GLType);
		void toggle_attribut(uint32_t _Count, bool _Enable = true);
		void bind();
		void unbind();

		template<size_t... Args>
		void set_attribut(size_t _Count, const float* _Data, unsigned _DrawMode)
		{
			bind();
			if (!mBuffer)	gl::GenBuffers(1, &mBuffer);
			gl::BindBuffer(GL_ARRAY_BUFFER, mBuffer);
			gl::BufferData(GL_ARRAY_BUFFER, static_cast<int64_t>(_Count * sizeof(float)), _Data, _DrawMode);
			
			mVertexSize = sizeof(float) * detail::unpack_sequence<Args...>::get_sum();
			auto offset = 0;
			for (auto i = 0; i < sizeof...(Args); ++i)
			{
				auto elem = detail::unpack_sequence<Args...>::get(i);
				gl::VertexAttribPointer(i, static_cast<int32_t>(elem), GL_FLOAT, GL_FALSE, static_cast<int32_t>(mVertexSize), 
					reinterpret_cast<const void*>(offset));
				gl::EnableVertexAttribArray(i);

				offset += static_cast<uint32_t>(sizeof(float) * elem);
			}
			unbind();
		}

	private:
		void _swap(Mesh& _Other) noexcept;
	};
}