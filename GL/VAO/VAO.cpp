#include "VAO.h"
#include <GL/GLFuncs.h>
#include <iostream>

namespace tgl
{
	VAO::VAO()
	{
		tgl::gl::genVertexArrays(1, &mVAO);
	}

	VAO::~VAO()
	{
		gl::deleteBuffers(mBuffers.size(), mBuffers.data());
		gl::deleteVertexArrays(1, &mVAO);
	}

	void VAO::bind()
	{
		gl::bindVertexArray(mVAO);
	}

	void VAO::unbind()
	{
		gl::bindVertexArray(0);
	}

	void VAO::draw(size_t count)
	{
		bind();
		for (auto i = 0; i < mBuffers.size(); ++i) gl::enableVertexAttribArray(i);
		gl::glDrawArrays(GL_TRIANGLES, 0, count);
		for (auto i = 0; i < mBuffers.size(); ++i) gl::enableVertexAttribArray(i);
	}

	void VAO::push_vbo(std::vector<float>& _Data)
	{
		add_vertex_buffer_object(_Data.data(), _Data.size());
	}
	
	void VAO::add_vertex_buffer_object(void* _Data, size_t _Count)
	{
		bind();
		unsigned vbo;
		tgl::gl::genBuffers(1, &vbo);
		tgl::gl::bindBuffer(GL_ARRAY_BUFFER, vbo);
		tgl::gl::bufferData(GL_ARRAY_BUFFER, _Count * sizeof(float), _Data, GL_STATIC_DRAW);
		tgl::gl::vertexArrtibPointer(mBuffers.size(), 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		mBuffers.push_back(vbo);
	}
}