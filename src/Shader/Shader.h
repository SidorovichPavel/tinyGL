#pragma once

#include <string>
#include <GL/GLFuncs.h>

namespace tgl
{
	class Shader
	{
		unsigned
			mProgram,
			mVertexShader,
			mFragmentShader;

		unsigned load_shader(const std::string& code, gl::GLenum shader_type);
	public:
		Shader(const std::string& shader_pack_name);
		~Shader();
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader(Shader&&) = default;
		Shader& operator=(Shader&&) = default;

		void link();
		void bind_attribute(unsigned index, const std::string& name);
		void use();

		void uniform_matrix4f(const std::string& name, void* ptr);
		void uniform_vector4f(const std::string& name, void* ptr);
		void uniform_vector3f(const std::string& name, void* ptr);

		void uniform_int(const std::string& name, int _Value);
	};


}