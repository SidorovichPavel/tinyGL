#include "Shader.hpp"

#include <iterator>
#include <stdexcept>
#include <fstream>

namespace tgl
{
	std::string Shader::path_prefix;

	Shader::Shader(const std::string& shader_pack_name)
	{
		mProgram = gl::CreateProgram();
		auto shader_path = path_prefix + shader_pack_name;
		mVertexShader = load_shader(shader_path + ".vert", GL_VERTEX_SHADER);
		mFragmentShader = load_shader(shader_path + ".frag", GL_FRAGMENT_SHADER);
		//auto GeometryShader = compile_shader(gsh_code, GL_GEOMETRY_SHADER);

		link();
	}

	Shader::Shader(std::string&& _Vert_Shader_Code, std::string&& _Frag_Shader_Code)
	{
		mProgram = gl::CreateProgram();
		mVertexShader = compile_shader(GL_VERTEX_SHADER, std::move(_Vert_Shader_Code));
		mFragmentShader = compile_shader(GL_FRAGMENT_SHADER, std::move(_Frag_Shader_Code));

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
		:
		mProgram(0),
		mVertexShader(0),
		mFragmentShader(0)
	{
		_swap(_Other);
	}

	Shader& Shader::operator=(Shader&& _Right) noexcept
	{
		_swap(_Right);
		return *this;
	}

	class InCType : public std::ctype<char>
	{
		mask mTable[table_size];
	public:
		using base = std::ctype<char>;
		InCType(size_t	refs = 0)
			: std::ctype<char>(mTable, false, refs)
		{
			std::copy_n(classic_table(), table_size, mTable);
			mTable['\n'] = base::punct;
			mTable[' '] = base::punct;
		}
	};

	unsigned Shader::load_shader(const std::string& shader_file_name, gl::GLenum shader_type)
	{
		std::ifstream fin(shader_file_name);
		if (!fin.is_open())
			throw std::runtime_error("file \"" + shader_file_name + "\" not found!");

		fin.imbue(std::locale(std::locale::classic(), new InCType));
		std::string shader_code = std::string(std::istream_iterator<char>(fin), std::istream_iterator<char>());

		return compile_shader(shader_type, std::move(shader_code));
	}

	unsigned Shader::compile_shader(gl::GLenum _Shader_Type, std::string&& _Code)
	{
		const char* c = _Code.c_str();
		unsigned shader = gl::CreateShader(_Shader_Type);
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

	void Shader::_swap(Shader& _Other)
	{
		std::swap(this->mProgram, _Other.mProgram);
		std::swap(this->mVertexShader, _Other.mVertexShader);
		std::swap(this->mFragmentShader, _Other.mFragmentShader);
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
