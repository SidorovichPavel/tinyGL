#pragma once

#include "Shader.hpp"

namespace tgl
{

	class FileShader : public Shader
	{
		unsigned load_shader(const std::string& _Path, gl::GLenum shader_type);

	public:
		FileShader(const std::string& _Pack_Name);
		FileShader(
			const std::string& _Vertex_File_Name,
			const std::string& _Fragment_File_Name,
			const std::string& _Geometry_File_Name);
		~FileShader() noexcept;
	};

}