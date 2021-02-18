#include <GL/Shader/Shader.h>

#include <iterator>
#include <stdexcept>
#include <fstream>

namespace tgl
{
	unsigned Shader::load_shader(const std::string& shader_file_name, gl::GLenum shader_type)
	{
		unsigned shader = gl::createShader(shader_type);
		std::ifstream fin(shader_file_name);
		if (!fin.is_open())
			throw std::runtime_error("file \"" + shader_file_name + "\" not found!");

		std::string shader_code = std::string(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());

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

	Shader::Shader(const std::string& shader_pack_name)
	{
		mProgram = gl::createProgram();
		mVertexShader = load_shader("res/glsl/" + shader_pack_name + ".vert", GL_VERTEX_SHADER);
		mFragmentShader = load_shader("res/glsl/" + shader_pack_name + ".frag", GL_FRAGMENT_SHADER);
		//auto GeometryShader = compile_shader(gsh_code, GL_GEOMETRY_SHADER);
	}

	void Shader::link()
	{
		gl::attachShader(mProgram, mVertexShader);
		gl::attachShader(mProgram, mFragmentShader);

		gl::linkProgram(mProgram);
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
		gl::detachShader(mProgram, mVertexShader);
		gl::detachShader(mProgram, mFragmentShader);
		gl::deleteShader(mVertexShader);
		gl::deleteShader(mFragmentShader);
		gl::deleteProgram(mProgram);
	}

	void Shader::uniform_matrix4f(const std::string& name, void* ptr)
	{
		auto matrix_loc = gl::getUniformLocation(mProgram, name.c_str());
		gl::uniformMatrix4fv(matrix_loc, 1, GL_FALSE, (const gl::GLfloat*)ptr);
	}
	void Shader::uniform_vector4f(const std::string& name, void* ptr)
	{
		auto loc = gl::getUniformLocation(mProgram, name.c_str());
		gl::uniformVector4fv(loc, 1, (const gl::GLfloat*)ptr);
	}
	void Shader::uniform_vector3f(const std::string& name, void* ptr)
	{
		auto loc = gl::getUniformLocation(mProgram, name.c_str());
		gl::uniformVector3fv(loc, 1, (const gl::GLfloat*)ptr);
	}
}
