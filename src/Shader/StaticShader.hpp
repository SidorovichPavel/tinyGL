#pragma once

#include "Shader.hpp"

namespace tgl
{
	class StaticShader : public Shader
	{
	public:
		StaticShader(
			const std::string& _Vertex_Code, 
			const std::string& _Fragment_Code, 
			const std::string& _Geometry_Code);
		~StaticShader();
	};


}