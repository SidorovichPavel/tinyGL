#include "Mesh.h"
#include <iostream>
#include <assert.h>

namespace tgl
{
	Mesh::Mesh()
		:
		mVertexSize(0),
		mIndicesBuffer(0),
		mIndicesCount(0),
		mBuffer(0),
		mVAO(0)
	{
		gl::GenVertexArrays(1, &mVAO);
	}

	Mesh::~Mesh()
	{
		if (mBuffer)
			gl::DeleteBuffers(1, &mBuffer);
		if (mIndicesBuffer)
			gl::DeleteBuffers(1, &mIndicesBuffer);
		if (mVAO)
			gl::DeleteVertexArrays(1, &mVAO);
	}

	Mesh::Mesh(Mesh&& _Other) noexcept
		:
		mVAO(0),
		mIndicesBuffer(0),
		mIndicesCount(0),
		mBuffer(0),
		mVertexSize(0)
	{
		std::swap(this->mVAO, _Other.mVAO);
		std::swap(this->mIndicesBuffer, _Other.mIndicesBuffer);
		std::swap(this->mIndicesCount, _Other.mIndicesCount);
		std::swap(this->mBuffer, _Other.mBuffer);
		std::swap(this->mVertexSize, _Other.mVertexSize);
	}

	Mesh& Mesh::operator=(Mesh&& _Right) noexcept
	{
		std::swap(this->mVAO, _Right.mVAO);
		std::swap(this->mIndicesBuffer, _Right.mIndicesBuffer);
		std::swap(this->mIndicesCount, _Right.mIndicesCount);
		std::swap(this->mBuffer, _Right.mBuffer);
		std::swap(this->mVertexSize, _Right.mVertexSize);
		return *this;
	}

	void Mesh::draw(uint32_t _GLType)
	{
		assert(mBuffer && mIndicesBuffer);

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
		assert(mVAO);
		gl::BindVertexArray(mVAO);
	}

	void Mesh::unbind()
	{
		gl::BindVertexArray(0);
	}

	void Mesh::_swap(Mesh& _Other) noexcept
	{
	}

	void Mesh::set_indices(size_t _Count, uint32_t* _Elems)
	{
		mIndicesCount = static_cast<int32_t>(_Count);

		bind();
		if (!mIndicesBuffer)
			gl::GenBuffers(1, &mIndicesBuffer);
		gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicesBuffer);
		gl::BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * mIndicesCount, _Elems, GL_STATIC_DRAW);
		unbind();
	}
}