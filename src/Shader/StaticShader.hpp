#pragma once

#include "Shader.hpp"

namespace tgl
{
	class StaticShader : public Shader
	{
		using base = Shader;
	public:
		StaticShader(
			const std::string& _Vertex_Code, 
			const std::string& _Fragment_Code, 
			const std::string& _Geometry_Code);
		~StaticShader();

		StaticShader(const StaticShader&) = delete;
		StaticShader& operator=(const StaticShader&) = delete;

		StaticShader(StaticShader&& _Other) noexcept;
		StaticShader& operator=(StaticShader&& _Right) noexcept;
	};
}