#include <src/Mesh/Mesh.h>
#include <iostream>
#include <assert.h>

namespace tgl
{
	Mesh::Mesh()
		:
		mVertexSize(0),
		mIndicesBuffer(0),
		mIndicesCount(0),
		mBuffer(0)
	{
		gl::genVertexArrays(1, &mVAO);
	}

	Mesh::~Mesh()
	{
		gl::deleteBuffers(1, &mBuffer);
		gl::deleteBuffers(1, &mIndicesBuffer);
		gl::deleteVertexArrays(1, &mVAO);
	}

	void Mesh::draw(uint32_t _GLType)
	{
		bind();
		gl::glDrawElements(_GLType, mIndicesCount, GL_UNSIGNED_INT, nullptr);
		unbind();
	}

	void Mesh::toggle_attribut(uint32_t _Count, bool _Enable)
	{
		_Enable
			? gl::enableVertexAttribArray(_Count)
			: gl::disableVertexAttribArray(_Count);
	}

	void Mesh::bind()
	{
		gl::bindVertexArray(mVAO);
	}

	void Mesh::unbind()
	{
		gl::bindVertexArray(0);
	}

	void Mesh::set_indices(size_t _Count, uint32_t* _Elems)
	{
		assert(mIndicesBuffer == 0);
		mIndicesCount = static_cast<int32_t>(_Count);

		bind();

		gl::genBuffers(1, &mIndicesBuffer);
		gl::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicesBuffer);
		gl::bufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * mIndicesCount, _Elems, GL_DYNAMIC_DRAW);

		unbind();
	}
}