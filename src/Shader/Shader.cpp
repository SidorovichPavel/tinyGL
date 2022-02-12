#include "Shader.hpp"

#include <iterator>
#include <stdexcept>
#include <fstream>

namespace tgl
{
	std::string Shader::path_prefix;

	Shader::Shader() noexcept
		:
		mVertexShader(0),
		mFragmentShader(0),
		mGeometryShader(0)
	{
		mProgram = gl::CreateProgram();
	}

	Shader::~Shader()
	{
		gl::DetachShader(mProgram, mVertexShader);
		gl::DeleteShader(mVertexShader);
		gl::DetachShader(mProgram, mFragmentShader);
		gl::DeleteShader(mFragmentShader);
		if (mGeometryShader)
		{
			gl::DetachShader(mProgram, mGeometryShader);
			gl::DeleteShader(mGeometryShader);
		}

		gl::DeleteProgram(mProgram);
	}

	unsigned Shader::compile_shader(gl::GLenum _Shader_Type, const std::string& _Code)
	{
		auto code = _Code.c_str();
		unsigned shader = gl::CreateShader(_Shader_Type);
		gl::ShaderSource(shader, 1, &code, nullptr);
		gl::CompileShader(shader);

		int status;
		gl::GetShaderiv(shader, GL_COMPILE_STATUS, &status);

		char log[0x1000];
		int length;
		gl::GetShaderInfoLog(shader, sizeof(log), &length, log);
		if (length <= 0)
			return shader;
		else
			return 0;
	}

	void Shader::link() noexcept
	{
		gl::AttachShader(mProgram, mVertexShader);
		gl::AttachShader(mProgram, mFragmentShader);
		if (mGeometryShader)
			gl::AttachShader(mProgram, mGeometryShader);

		gl::LinkProgram(mProgram);
	}



	void Shader::bind_attribute(unsigned index, const std::string& name)
	{
		gl::BindAttribLocation(mProgram, index, name.c_str());
	}

	void Shader::use()
	{
		gl::UseProgram(mProgram);
	}

	//uniforms

	void Shader::uniform_matrix4f(const std::string& name, void* ptr)
	{
		auto loc = gl::GetUniformLocation(mProgram, name.c_str());
		gl::UniformMatrix4fv(loc, 1, GL_FALSE, (const gl::GLfloat*)ptr);
	}
	void Shader::uniform_vector4f(const std::string& name, void* ptr)
	{
		auto loc = gl::GetUniformLocation(mProgram, name.c_str());
		gl::Uniform4fv(loc, 1, (const gl::GLfloat*)ptr);
	}
	void Shader::uniform_vector3f(const std::string& name, void* ptr)
	{
		auto loc = gl::GetUniformLocation(mProgram, name.c_str());
		gl::Uniform3fv(loc, 1, (const gl::GLfloat*)ptr);
	}
	void Shader::uniform_int(const std::string& name, int _Value)
	{
		auto loc = gl::GetUniformLocation(mProgram, name.c_str());
		gl::Uniform1i(loc, _Value);
	}
}
