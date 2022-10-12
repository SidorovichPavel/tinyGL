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

		template<class T>
		struct type_to_gl {
			constexpr static unsigned value = 0;
		};
		template<>
		struct type_to_gl<float> {
			constexpr static unsigned value = GL_FLOAT;
		};
		template<>
		struct type_to_gl<unsigned int> {
			constexpr static unsigned value = GL_UNSIGNED_INT;
		};
		template<>
		struct type_to_gl<int> {
			constexpr static unsigned value = GL_INT;
		};

	}

	class Mesh
	{
	public:

		enum class GlDrawMode : uint32_t
		{
			Static = GL_STATIC_DRAW,
			Dynamic = GL_DYNAMIC_DRAW,
			Stream = GL_STREAM_DRAW
		};

		enum class GlDrawObject : uint32_t
		{
			Points = GL_POINTS,
			Lines = GL_LINES,
			LineStrip = GL_LINE_STRIP,
			LineLoop = GL_LINE_LOOP,
			Triangles = GL_TRIANGLES,
			TriangleStrip = GL_TRIANGLE_STRIP,
			TrianglesFan = GL_TRIANGLE_FAN,
		};

		Mesh();
		~Mesh();
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh(Mesh&& _Other) noexcept;
		Mesh& operator=(Mesh&& _Right) noexcept;

		
		void draw(GlDrawObject _GLType = GlDrawObject::Triangles);
		void toggle_attribut(uint32_t _Count, bool _Enable = true);
		void bind();
		void unbind();

		void set_indices(size_t _Count, uint32_t* _Elems, GlDrawMode _DrawMode = GlDrawMode::Static);

		template<class T>
		void set_attribut(size_t _Idx, size_t _NumbOfElemPerVertex, size_t _Count, const T* _Data, GlDrawMode _DrawMode = GlDrawMode::Static)
		{
			bind();
			if (_Idx == mBuffer.size())
			{
				mBuffer.push_back(0);
				gl::GenBuffers(1, &mBuffer[_Idx]);
			}
			else if (_Idx > mBuffer.size())
				return;

			auto buffer_size = static_cast<tgl::gl::GLsizeiptr>(sizeof(T) * _Count);
			gl::BindBuffer(GL_ARRAY_BUFFER, mBuffer[_Idx]);
			gl::BufferData(GL_ARRAY_BUFFER, buffer_size, _Data, static_cast<uint32_t>(_DrawMode));

			mVertexSize = _NumbOfElemPerVertex * sizeof(T);

			gl::VertexAttribPointer(static_cast<int32_t>(_Idx), static_cast<int32_t>(_NumbOfElemPerVertex), detail::type_to_gl<T>::value, GL_FALSE,
				static_cast<int32_t>(mVertexSize), reinterpret_cast<const void*>(0));
			gl::EnableVertexAttribArray(static_cast<int32_t>(_Idx));

			unbind();
		}

		template<uint32_t... Args>
		void set_attributs(size_t _Count, const float* _Data, GlDrawMode _DrawMode = GlDrawMode::Static)
		{
			bind();
			if (!mBuffer.size())
			{
				mBuffer.push_back(0);
				gl::GenBuffers(1, mBuffer.data());
			}
			gl::BindBuffer(GL_ARRAY_BUFFER, mBuffer.front());
			gl::BufferData(GL_ARRAY_BUFFER, static_cast<int64_t>(_Count * sizeof(float)), _Data, static_cast<uint32_t>(_DrawMode));

			mVertexSize = sizeof(float) * detail::unpack_sequence<Args...>::get_sum();
			size_t offset = 0;
			for (auto i = 0; i < sizeof...(Args); ++i)
			{
				auto elem = detail::unpack_sequence<Args...>::get(i);
				gl::VertexAttribPointer(i, static_cast<int32_t>(elem), GL_FLOAT, GL_FALSE,
					static_cast<int32_t>(mVertexSize), reinterpret_cast<const void*>(offset));
				gl::EnableVertexAttribArray(i);

				offset += static_cast<size_t>(sizeof(float) * elem);
			}
			unbind();
		}


		
	private:
		void _swap(Mesh& _Other) noexcept;

		uint32_t				mVAO;
		uint32_t				mIndicesBuffer;
		int32_t					mIndicesCount;
		std::vector<uint32_t>	mBuffer;
		size_t					mVertexSize;

	};
}