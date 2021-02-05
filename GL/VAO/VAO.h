#pragma once

#include <vector>
#include <array>

namespace tgl
{
	class VAO
	{
		unsigned mVAO;
		std::vector<unsigned> mBuffers;

	public:
		VAO();
		VAO(const VAO& _Right) = delete;
		VAO(VAO&& _Right) = delete;
		~VAO();

		void bind();
		static void unbind();
		void draw(size_t count);

		void add_vertex_buffer_object(void* _Data, size_t _Count);
		template<size_t N>
		void push_vbo(std::array<float, N>& _Data)
		{
			add_vertex_buffer_object(_Data.data(), _Data.size());
		}
		void push_vbo(std::vector<float>& _Data);
	};
}