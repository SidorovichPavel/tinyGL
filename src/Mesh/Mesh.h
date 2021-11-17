#pragma once

#include <vector>
#include <array>
#include <GL/GLFuncs.h>

#pragma warning(disable:4312)

namespace tgl
{
	namespace detail
	{
		template<size_t... _Args>
		struct unpack_sequence
		{
			constexpr static size_t Elems[sizeof...(_Args)] = { _Args... };
			constexpr static size_t get_sum()
			{
				size_t res = 0;
				for (auto i = 0; i < sizeof...(_Args); ++i)
					res += Elems[i];
				return res;
			}
			constexpr static size_t get(size_t _Index)
			{
				return Elems[_Index];
			}
			constexpr static size_t count = sizeof...(_Args);
		};
	}

	class Mesh
	{
		uint32_t				mVAO;
		uint32_t				mIndicesBuffer;
		int32_t					mIndicesCount;
		uint32_t				mBuffer;
		size_t					mVertexSize;
	public:
		Mesh();
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh(Mesh&& _Other) noexcept;
		Mesh& operator=(Mesh&& _Right) noexcept;
		~Mesh();

		void set_indices(size_t _Count, uint32_t* _Elems);

		void draw(uint32_t _GLType);

		void toggle_attribut(uint32_t _Count, bool _Enable);

		void bind();
		void unbind();

		template<size_t... _Args>
		void set_attribut(size_t _Count, const float* _Data)
		{
			bind();

			if (!mBuffer)	gl::GenBuffers(1, &mBuffer);
			gl::BindBuffer(GL_ARRAY_BUFFER, mBuffer);
			gl::BufferData(GL_ARRAY_BUFFER, int64_t(_Count * sizeof(float)), _Data, GL_STATIC_DRAW);

			mVertexSize = sizeof(float) * detail::unpack_sequence<_Args...>::get_sum();
			auto offset = 0;
			for (auto i = 0; i < sizeof...(_Args); ++i)
			{
				auto elem = detail::unpack_sequence<_Args...>::get(i);
				gl::VertexAttribPointer(i, (int32_t)elem, GL_FLOAT, GL_FALSE, (uint32_t)mVertexSize, reinterpret_cast<const void*>(offset));
				gl::EnableVertexAttribArray(i);

				offset += static_cast<uint32_t>(sizeof(float) * elem);
			}

			unbind();
		}
	};
}