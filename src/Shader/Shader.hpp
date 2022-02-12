#pragma once

#include <string>
#include "../../GL/GLFuncs.hpp"

namespace tgl
{
	class Shader
	{
	protected:

		unsigned
			mProgram,
			mVertexShader,
			mFragmentShader,
			mGeometryShader;

		unsigned compile_shader(gl::GLenum _Shader_Type, const std::string& _Code);

	public:
		static std::string path_prefix;

		Shader() noexcept;
		virtual ~Shader();
		
		Shader(Shader&& _Other) = delete;
		Shader& operator=(Shader&& _Right) = delete;

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
		
		void link() noexcept;
		void bind_attribute(unsigned _Index, const std::string& _Name);
		void use();

		void uniform_matrix4f(const std::string& name, void* ptr);
		void uniform_vector4f(const std::string& name, void* ptr);
		void uniform_vector3f(const std::string& name, void* ptr);

		void uniform_int(const std::string& name, int _Value);
	};


}