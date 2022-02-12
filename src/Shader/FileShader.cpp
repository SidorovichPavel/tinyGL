#include "FileShader.hpp"
#include <fstream>

namespace tgl
{

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

	unsigned FileShader::load_shader(const std::string& shader_file_name, gl::GLenum shader_type)
	{
		std::ifstream fin(shader_file_name);
		if (!fin.is_open())
			return 0;

		fin.imbue(std::locale(std::locale::classic(), new InCType));
		std::string shader_code = std::string(std::istream_iterator<char>(fin), std::istream_iterator<char>());

		return compile_shader(shader_type, shader_code);
	}

	FileShader::FileShader(const std::string& _Pack_Name) 
		: 
		Shader()
	{
		auto shader_path = path_prefix;
		shader_path += _Pack_Name;

		mVertexShader = load_shader(shader_path + ".vert", GL_VERTEX_SHADER);
		mFragmentShader = load_shader(shader_path + ".frag", GL_FRAGMENT_SHADER);
		mGeometryShader = load_shader(shader_path + ".geom", GL_GEOMETRY_SHADER);

		link();
	}

	FileShader::FileShader(const std::string& _Vertex_File_Name, const std::string& _Fragment_File_Name, const std::string& _Geometry_File_Name)
		:
		Shader()
	{
		mVertexShader = load_shader(path_prefix + _Vertex_File_Name, GL_VERTEX_SHADER);
		mFragmentShader = load_shader(path_prefix + _Fragment_File_Name, GL_FRAGMENT_SHADER);
		mGeometryShader = load_shader(path_prefix + _Geometry_File_Name, GL_GEOMETRY_SHADER);

		link();
	}

	FileShader::~FileShader() noexcept
	{
	}

}