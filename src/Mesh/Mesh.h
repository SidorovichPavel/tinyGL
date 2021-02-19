#pragma once

#include <vector>
#include <array>
#include <GL/GLFuncs.h>

namespace tgl
{
	namespace hide
	{
		template<size_t... _Args>
		struct unpack_sequence
		{
			constexpr static size_t _Elems[sizeof...(_Args)] = { _Args... };
			constexpr static size_t get_sum()
			{
				size_t res = 0;
				for (auto i = 0; i < sizeof...(_Args); ++i)
					res += _Elems[i];
				return res;
			}
			constexpr static size_t get(size_t _Index)
			{
				return _Elems[_Index];
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
		Mesh(const Mesh& _Right) = delete;
		Mesh(Mesh&& _Right) = delete;
		~Mesh();

		void set_indices(size_t _Count, uint32_t* _Elems);
		void draw(uint32_t _GLType);

		void toggle_attribut(uint32_t _Count, bool _Enable);

		void bind();
		void unbind();


		template<size_t... _Args>
		void add_attribut(size_t _Count, const float* _Data)
		{
			gl::genBuffers(1, &mBuffer);
			gl::bindBuffer(GL_ARRAY_BUFFER, mBuffer);
			gl::bufferData(GL_ARRAY_BUFFER, int64_t(_Count * sizeof(float)), _Data, GL_DYNAMIC_DRAW);

			mVertexSize = sizeof(float) * hide::unpack_sequence<_Args...>::get_sum();
			auto offset = 0;
			for (auto i = 0; i < sizeof...(_Args); ++i)
			{
				auto elem = hide::unpack_sequence<_Args...>::get(i);
				gl::vertexArrtibPointer(i, (int32_t)elem, GL_FLOAT, GL_FALSE, (uint32_t)mVertexSize, reinterpret_cast<const void*>(offset));
				gl::enableVertexAttribArray(i);

				offset += static_cast<uint32_t>(sizeof(float) * elem);
			}
		}

	};
}