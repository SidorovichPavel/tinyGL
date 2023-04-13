#pragma once

#include "..\src\Utility\function.hpp"

#include <string>
#include <stdexcept>

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
		#include <gl\GL.h>
		#include "glcorearb.h"
		#include "glext.h"

		template<class>
		struct tgl_func
		{	};

		template<class ReturnType, class... ArgTypes>
		struct tgl_func<ReturnType(_stdcall*)(ArgTypes...)>
		{
			using func_t = fnw::function<ReturnType(ArgTypes...)>;

			static func_t LoadFunction(const char* func_name)
			{
				void* data = tgl::win::wglGetProcAddress(func_name);
				return static_cast<ReturnType(_stdcall*)(ArgTypes...)>(data);
			}
		};
		
		#define GL_EXTERN_DECL(type, name) extern tgl_func<type>::func_t name

		GL_EXTERN_DECL(PFNGLGENBUFFERSPROC, gen_buffers);
		GL_EXTERN_DECL(PFNGLBINDBUFFERPROC, bind_buffer);
		GL_EXTERN_DECL(PFNGLBUFFERDATAPROC, buffer_data);
		GL_EXTERN_DECL(PFNGLDELETEBUFFERSPROC, delete_buffers);

		GL_EXTERN_DECL(PFNGLGENVERTEXARRAYSPROC, gen_vertex_arrays);
		GL_EXTERN_DECL(PFNGLDELETEVERTEXARRAYSPROC, delete_vertex_arrays);
		GL_EXTERN_DECL(PFNGLVERTEXATTRIBPOINTERPROC, vertex_attrib_pointer);
		GL_EXTERN_DECL(PFNGLBINDVERTEXARRAYPROC, bind_vertex_array);
		GL_EXTERN_DECL(PFNGLENABLEVERTEXATTRIBARRAYPROC, enable_vertex_attrib_array);
		GL_EXTERN_DECL(PFNGLDISABLEVERTEXATTRIBARRAYPROC, disable_vertex_attrib_array);

		GL_EXTERN_DECL(PFNGLCREATEPROGRAMPROC, create_program);
		GL_EXTERN_DECL(PFNGLDELETEPROGRAMPROC, delete_program);
		GL_EXTERN_DECL(PFNGLCREATESHADERPROC, create_shader);
		GL_EXTERN_DECL(PFNGLSHADERSOURCEPROC, shader_source);
		GL_EXTERN_DECL(PFNGLCOMPILESHADERPROC, compile_shader);
		GL_EXTERN_DECL(PFNGLGETSHADERIVPROC, get_shaderiv);
		GL_EXTERN_DECL(PFNGLATTACHSHADERPROC, attach_shader);
		GL_EXTERN_DECL(PFNGLLINKPROGRAMPROC, link_program);
		GL_EXTERN_DECL(PFNGLUSEPROGRAMPROC, use_program);
		GL_EXTERN_DECL(PFNGLDELETESHADERPROC, delete_shader);
		GL_EXTERN_DECL(PFNGLDETACHSHADERPROC, detach_shader);
		GL_EXTERN_DECL(PFNGLGETSHADERINFOLOGPROC, get_shader_info_log);
		GL_EXTERN_DECL(PFNGLBINDATTRIBLOCATIONPROC, bind_attrib_location);
		GL_EXTERN_DECL(PFNGLGETUNIFORMLOCATIONPROC, get_uniform_location);

		GL_EXTERN_DECL(PFNGLUNIFORMMATRIX4FVPROC, uniform_matrix_4fv);
		GL_EXTERN_DECL(PFNGLUNIFORM4FVPROC, uniform_4fv);
		GL_EXTERN_DECL(PFNGLUNIFORM3FVPROC, uniform_3fv);
		GL_EXTERN_DECL(PFNGLUNIFORM4FPROC, uniform_4f);
		GL_EXTERN_DECL(PFNGLUNIFORM1IPROC, uniform_1i);

		GL_EXTERN_DECL(PFNGLACTIVETEXTUREPROC, active_texture);
		GL_EXTERN_DECL(PFNGLGENERATEMIPMAPPROC, generate_mipmap);

		GL_EXTERN_DECL(PFNGLDEBUGMESSAGECALLBACKPROC, debug_message_callback);

		#undef GL_EXTERN_DECL
	}
}