#pragma once

#include <string>
#include <stdexcept>

namespace tgl::win
{
	#include <Windows.h>
}

namespace tgl::gl
{
	#include <GL/GL.h>
	#include <GL/glext.h>
	//vbo
	extern PFNGLGENBUFFERSPROC genBuffers;
	extern PFNGLBINDBUFFERPROC bindBuffer;
	extern PFNGLBUFFERDATAPROC bufferData;
	extern PFNGLDELETEBUFFERSPROC deleteBuffers;
	//vbo end
	//vao
	extern PFNGLGENVERTEXARRAYSPROC genVertexArrays;
	extern PFNGLVERTEXATTRIBPOINTERPROC vertexArrtibPointer;
	extern PFNGLBINDVERTEXARRAYPROC bindVertexArray;
	extern PFNGLENABLEVERTEXATTRIBARRAYPROC enableVertexAttribArray;
	extern PFNGLDISABLEVERTEXATTRIBARRAYPROC disableVertexAttribArray;
	extern PFNGLDELETEVERTEXARRAYSPROC deleteVertexArrays;
	//vao end
	//program
	extern PFNGLCREATEPROGRAMPROC createProgram;
	extern PFNGLDELETEPROGRAMPROC deleteProgram;
	extern PFNGLCREATESHADERPROC createShader;
	extern PFNGLSHADERSOURCEPROC shaderSource;
	extern PFNGLGETSHADERIVPROC getShaderiv;
	extern PFNGLATTACHSHADERPROC attachShader;
	extern PFNGLLINKPROGRAMPROC linkProgram;
	extern PFNGLUSEPROGRAMPROC useProgram;
	extern PFNGLGETSHADERINFOLOGPROC getShaderInfoLog;
	extern PFNGLBINDATTRIBLOCATIONPROC bindAttribLocation;
	//program end

	template<class T>
	T LoadFunction(const char* _Func_Name)
	{
		void* data = tgl::win::wglGetProcAddress(_Func_Name);
		if (data)
		{
			return reinterpret_cast<T>(data);
		}
		else
		{
			std::string error_str("tinyGL[Win32] -> init failed -> failed wile get proc address \"");
			error_str += _Func_Name;
			error_str += "\"";
			throw std::runtime_error(error_str);
		}
		return nullptr;
	}
}