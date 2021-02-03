#include "Shader.h"

#include <fstream>
#include <iterator>
#include <stdexcept>

namespace tgl
{
	unsigned Shader::load_shader(const std::string& path, gl::GLenum shader_type)
	{
		unsigned shader = gl::createShader(shader_type);
		std::ifstream fin(path);

		std::string shader_code{ std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>() };

		const char* c = shader_code.c_str();
		gl::shaderSource(shader, 1, &c, nullptr);
		gl::compileShader(shader);

		int status;
		gl::getShaderiv(shader, GL_COMPILE_STATUS, &status);
		char log[0x1000];
		int length;
		gl::getShaderInfoLog(shader, sizeof(log), &length, log);
		if (length <= 0)
			return shader;
		else
			throw std::runtime_error(log);
	}

	Shader::Shader(const std::string& name)
	{
		mProgram = gl::createProgram();
		mVertexShader = load_shader("res/glsl/" + name + ".vert", GL_VERTEX_SHADER);
		mFragmentShader = load_shader("res/glsl/" + name + ".frag", GL_FRAGMENT_SHADER);
	}

	void Shader::link()
	{
		gl::attachShader(mProgram, mVertexShader);
		gl::attachShader(mProgram, mFragmentShader);

		gl::linkProgram(mProgram);

		gl::deleteShader(mVertexShader);
		gl::deleteShader(mFragmentShader);
	}

	void Shader::bind_attribute(unsigned index, const std::string& name)
	{
		gl::bindAttribLocation(mProgram, index, name.c_str());
	}

	void Shader::use()
	{
		gl::useProgram(mProgram);
	}

	Shader::~Shader()
	{
		gl::deleteProgram(mProgram);
	}
}
