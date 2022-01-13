#pragma once

#include <string>
#include "../../GL/GLFuncs.hpp"

namespace tgl
{
	class Shader
	{
		unsigned
			mProgram,
			mVertexShader,
			mFragmentShader;

		unsigned load_shader(const std::string& _Path, gl::GLenum shader_type);
		unsigned compile_shader(gl::GLenum _Shader_Type, std::string&& _Code);
		void _swap(Shader& _Other);
	public:
		static std::string path_prefix;

		void link() noexcept;
		Shader(const std::string& shader_pack_name);
		Shader(std::string&& _Vert_Shader_Code, std::string&& _Frag_Shader_Code);
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