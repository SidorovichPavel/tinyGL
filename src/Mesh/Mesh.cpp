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
		gl::GenVertexArrays(1, &mVAO);
	}

	Mesh::~Mesh()
	{
		gl::DeleteBuffers(1, &mBuffer);
		gl::DeleteBuffers(1, &mIndicesBuffer);
		gl::DeleteVertexArrays(1, &mVAO);
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
			? gl::EnableVertexAttribArray(_Count)
			: gl::DisableVertexAttribArray(_Count);
	}

	void Mesh::bind()
	{
		gl::BindVertexArray(mVAO);
	}

	void Mesh::unbind()
	{
		gl::BindVertexArray(0);
	}

	void Mesh::set_indices(size_t _Count, uint32_t* _Elems)
	{
		assert(mIndicesBuffer == 0);
		mIndicesCount = static_cast<int32_t>(_Count);

		bind();

		gl::GenBuffers(1, &mIndicesBuffer);
		gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicesBuffer);
		gl::BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * mIndicesCount, _Elems, GL_STATIC_DRAW);

		unbind();
	}
}