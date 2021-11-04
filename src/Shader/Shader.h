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
		void link() noexcept;
		Shader(const std::string& shader_pack_name);
		~Shader();
		
		Shader(Shader&& _Other) noexcept;
		Shader& operator=(Shader&& _Right) noexcept;

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
		
		void bind_attribute(unsigned _Index, const std::string& _Name);
		void use();

		void uniform_matrix4f(const std::string& name, void* ptr);
		void uniform_vector4f(const std::string& name, void* ptr);
		void uniform_vector3f(const std::string& name, void* ptr);

		void uniform_int(const std::string& name, int _Value);
	};


}