#include "VAO.h"
#include <GL/GLFuncs.h>
#include <iostream>
#include <assert.h>
namespace tgl
{
	VAO::VAO()
	{
		gl::genVertexArrays(1, &mVAO);
	}

	VAO::~VAO()
	{
		gl::deleteBuffers((gl::GLsizei)mBuffers.size(), mBuffers.data());
		gl::deleteBuffers(1, &mIndicesBuffer);
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

	void VAO::draw(size_t type)
	{
		bind();
		for (auto i = 0; i < mBuffers.size(); ++i) gl::enableVertexAttribArray(i);
		assert(mIndicesBuffer != 0);
		gl::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicesBuffer);
		gl::glDrawElements(type, mIndicesCount, GL_UNSIGNED_INT, nullptr);
		for (auto i = 0; i < mBuffers.size(); ++i) gl::enableVertexAttribArray(i);
		unbind();
	}

	void VAO::push_vbo(std::vector<float>& _Data)
	{
		add_vertex_buffer(_Data.data(), _Data.size());
	}
	
	void VAO::add_vertex_buffer(void* _Data, size_t _Count)
	{
		bind();
		unsigned vbo;
		gl::genBuffers(1, &vbo);
		gl::bindBuffer(GL_ARRAY_BUFFER, vbo);
		gl::bufferData(GL_ARRAY_BUFFER, _Count * sizeof(float), _Data, GL_STATIC_DRAW);
		gl::vertexArrtibPointer((gl::GLsizei)mBuffers.size(), 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		mBuffers.push_back(vbo);
		unbind();
	}
	void VAO::add_indices_buffer(void* _Data, size_t _Count)
	{
		assert(mIndicesBuffer == 0);
		mIndicesCount = _Count;
		bind();
		tgl::gl::genBuffers(1, &mIndicesBuffer);
		tgl::gl::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicesBuffer);
		tgl::gl::bufferData(GL_ELEMENT_ARRAY_BUFFER, _Count * sizeof(unsigned), _Data, GL_STATIC_DRAW);
		unbind();
	}
}