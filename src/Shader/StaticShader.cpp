#include "StaticShader.hpp"

namespace tgl
{

	StaticShader::StaticShader(const std::string& _Vertex_Code, const std::string& _Fragment_Code, const std::string& _Geometry_Code)
	{
		mVertexShader = compile_shader(GL_VERTEX_SHADER, _Vertex_Code);
		mFragmentShader = compile_shader(GL_FRAGMENT_SHADER, _Fragment_Code);
		mGeometryShader = compile_shader(GL_GEOMETRY_SHADER, _Geometry_Code);

		link();
	}

	StaticShader::~StaticShader()
	{

	}

}