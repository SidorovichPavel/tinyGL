#pragma once

#include <string>

#include <GL/GLFuncs.h>

namespace tgl
{
	class Shader
	{
		unsigned mProgram,
			mVertexShader,
			mFragmentShader;

		unsigned load_shader(const std::string& path, gl::GLenum shader_type);
	public:
		Shader(const std::string&);
		~Shader();
		Shader(const Shader&) = default;
		Shader(Shader&&) = default;

		void link();
		void bind_attribute(unsigned index, const std::string& name);
		void use();


	};


}