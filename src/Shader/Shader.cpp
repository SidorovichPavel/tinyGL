#include <src/Shader/Shader.h>

#include <iterator>
#include <stdexcept>
#include <fstream>

namespace tgl
{
	Shader::Shader(const std::string& shader_pack_name)
	{
		mProgram = gl::CreateProgram();
		mVertexShader = load_shader("res/glsl/" + shader_pack_name + ".vert", GL_VERTEX_SHADER);
		mFragmentShader = load_shader("res/glsl/" + shader_pack_name + ".frag", GL_FRAGMENT_SHADER);
		//auto GeometryShader = compile_shader(gsh_code, GL_GEOMETRY_SHADER);

		link();
	}

	Shader::~Shader()
	{
		gl::DetachShader(mProgram, mVertexShader);
		gl::DetachShader(mProgram, mFragmentShader);
		gl::DeleteShader(mVertexShader);
		gl::DeleteShader(mFragmentShader);
		gl::DeleteProgram(mProgram);
	}

	Shader::Shader(Shader&& _Other) noexcept
	{
		mProgram = _Other.mProgram; 
		_Other.mProgram = 0;
		
		mVertexShader = _Other.mVertexShader;
		_Other.mVertexShader = 0;

		mFragmentShader = _Other.mFragmentShader;
		_Other.mFragmentShader = 0;
	}

	Shader& Shader::operator=(Shader&& _Right) noexcept
	{
		mProgram = _Right.mProgram;
		_Right.mProgram = 0;

		mVertexShader = _Right.mVertexShader;
		_Right.mVertexShader = 0;

		mFragmentShader = _Right.mFragmentShader;
		_Right.mFragmentShader = 0;

		return *this;
	}

	unsigned Shader::load_shader(const std::string& shader_file_name, gl::GLenum shader_type)
	{
		unsigned shader = gl::CreateShader(shader_type);
		std::ifstream fin(shader_file_name);
		if (!fin.is_open())
			throw std::runtime_error("file \"" + shader_file_name + "\" not found!");

		std::string shader_code = std::string(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());

		const char* c = shader_code.c_str();
		gl::ShaderSource(shader, 1, &c, nullptr);
		gl::CompileShader(shader);

		int status;
		gl::GetShaderiv(shader, GL_COMPILE_STATUS, &status);
		char log[0x1000];
		int length;
		gl::GetShaderInfoLog(shader, sizeof(log), &length, log);
		if (length <= 0)
			return shader;
		else
			throw std::runtime_error(log);
	}

	void Shader::link() noexcept
	{
		gl::AttachShader(mProgram, mVertexShader);
		gl::AttachShader(mProgram, mFragmentShader);

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
		auto matrix_loc = gl::GetUniformLocation(mProgram, name.c_str());
		gl::UniformMatrix4fv(matrix_loc, 1, GL_FALSE, (const gl::GLfloat*)ptr);
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
