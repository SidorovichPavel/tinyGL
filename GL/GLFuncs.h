#pragma once

#include <string>
#include <stdexcept>
#include <functional>

namespace tgl
{
	#ifdef _WIN32
	namespace win
	{
		#include <Windows.h>
	}
	#endif
}

namespace tgl
{
	namespace gl
	{
		#include "GL/GL.h"
		#include "GL/glext.h"

		template<class>
		struct tgl_func {};

		template<class Ret, class... Params>
		struct tgl_func<Ret(__stdcall*)(Params...)>
		{
			using func_t = Ret(Params...);

			static std::function<Ret(Params...)> LoadFunction(const char* _Func_Name)
			{
				void* data = tgl::win::wglGetProcAddress(_Func_Name);
				if (data)
					return reinterpret_cast<Ret(*)(Params...)>(data);

				return nullptr;
			}
		};

		#define GL_EXTERN_DECL(type, name) extern std::function<typename tgl_func< type >::func_t> name

		GL_EXTERN_DECL(PFNGLGENBUFFERSPROC, GenBuffers);
		GL_EXTERN_DECL(PFNGLBINDBUFFERPROC, BindBuffer);
		GL_EXTERN_DECL(PFNGLBUFFERDATAPROC, BufferData);
		GL_EXTERN_DECL(PFNGLDELETEBUFFERSPROC, DeleteBuffers);

		GL_EXTERN_DECL(PFNGLGENVERTEXARRAYSPROC, GenVertexArrays);
		GL_EXTERN_DECL(PFNGLVERTEXATTRIBPOINTERPROC, VertexAttribPointer);
		GL_EXTERN_DECL(PFNGLBINDVERTEXARRAYPROC, BindVertexArray);
		GL_EXTERN_DECL(PFNGLENABLEVERTEXATTRIBARRAYPROC, EnableVertexAttribArray);
		GL_EXTERN_DECL(PFNGLDISABLEVERTEXATTRIBARRAYPROC, DisableVertexAttribArray);
		GL_EXTERN_DECL(PFNGLDELETEVERTEXARRAYSPROC, DeleteVertexArrays);

		GL_EXTERN_DECL(PFNGLCREATEPROGRAMPROC, CreateProgram);
		GL_EXTERN_DECL(PFNGLDELETEPROGRAMPROC, DeleteProgram);
		GL_EXTERN_DECL(PFNGLCREATESHADERPROC, CreateShader);
		GL_EXTERN_DECL(PFNGLSHADERSOURCEPROC, ShaderSource);
		GL_EXTERN_DECL(PFNGLCOMPILESHADERPROC, CompileShader);
		GL_EXTERN_DECL(PFNGLGETSHADERIVPROC, GetShaderiv);
		GL_EXTERN_DECL(PFNGLATTACHSHADERPROC, AttachShader);
		GL_EXTERN_DECL(PFNGLLINKPROGRAMPROC, LinkProgram);
		GL_EXTERN_DECL(PFNGLUSEPROGRAMPROC, UseProgram);
		GL_EXTERN_DECL(PFNGLDELETESHADERPROC, DeleteShader);
		GL_EXTERN_DECL(PFNGLDETACHSHADERPROC, DetachShader);
		GL_EXTERN_DECL(PFNGLGETSHADERINFOLOGPROC, GetShaderInfoLog);
		GL_EXTERN_DECL(PFNGLBINDATTRIBLOCATIONPROC, BindAttribLocation);
		GL_EXTERN_DECL(PFNGLGETUNIFORMLOCATIONPROC, GetUniformLocation);

		GL_EXTERN_DECL(PFNGLUNIFORMMATRIX4FVPROC, UniformMatrix4fv);
		GL_EXTERN_DECL(PFNGLUNIFORM4FVPROC, Uniform4fv);
		GL_EXTERN_DECL(PFNGLUNIFORM3FVPROC, Uniform3fv);
		GL_EXTERN_DECL(PFNGLUNIFORM1IPROC, Uniform1i);

		GL_EXTERN_DECL(PFNGLACTIVETEXTUREPROC, ActiveTexture);
		GL_EXTERN_DECL(PFNGLGENERATEMIPMAPPROC, GenerateMipmap);


		GL_EXTERN_DECL(PFNGLDEBUGMESSAGECALLBACKPROC, DebugMessageCallback);

		#undef GL_EXTERN_DECL
	}
}