#include <src/tinyGL.h>
#include <GL/GLFuncs.h>

#include <stdexcept>
#include <iostream>

namespace tgl
{
	void event_pool() noexcept
	{
	#ifdef _WIN32
		win::MSG msg;
		if (win::PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE))
		{
			win::GetMessage(&msg, nullptr, 0, 0);
			win::TranslateMessage(&msg);
			win::DispatchMessage(&msg);
		}
	#else
		//TODO
	#endif
	}

	void Init()
	{
#ifdef _WIN32
		using namespace win;
		HWND handle = CreateWindowEx(0, L"button", L"", WS_POPUP, 0, 0, 0, 0, 0, 0, 0, 0);
		if (!handle)
			throw std::runtime_error("tinyGL[Win32] -> init failed -> failed wile window init");

		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
			PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
			32,                   // Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,                   // Number of bits for the depthbuffer
			8,                    // Number of bits for the stencilbuffer
			0,                    // Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		HDC dc = GetDC(handle);
		int pixel_format = ChoosePixelFormat(dc, &pfd);
		if (!pixel_format)
			throw std::runtime_error("tinyGL[Win32] -> init failed -> feiled wile pixel format choose");

		if (!SetPixelFormat(dc, pixel_format, &pfd))
			throw std::runtime_error("tinyGL[Win32] -> init failed -> failed wile set pixel format");

		HGLRC gl_rc = wglCreateContext(dc);
		wglMakeCurrent(dc, gl_rc);
		
		std::cout << "Loading OpenGL extentions..." << std::endl;

		gl::genVertexArrays = gl::LoadFunction<gl::PFNGLGENVERTEXARRAYSPROC>("glGenVertexArrays");
		gl::vertexArrtibPointer = gl::LoadFunction<gl::PFNGLVERTEXATTRIBPOINTERPROC>("glVertexAttribPointer");
		gl::bindVertexArray = gl::LoadFunction<gl::PFNGLBINDVERTEXARRAYPROC>("glBindVertexArray");
		gl::enableVertexAttribArray = gl::LoadFunction<gl::PFNGLENABLEVERTEXATTRIBARRAYPROC>("glEnableVertexAttribArray");
		gl::disableVertexAttribArray = gl::LoadFunction<gl::PFNGLDISABLEVERTEXATTRIBARRAYPROC>("glDisableVertexAttribArray");
		gl::deleteVertexArrays = gl::LoadFunction<gl::PFNGLDELETEVERTEXARRAYSPROC>("glDeleteVertexArrays");

		gl::genBuffers = gl::LoadFunction<gl::PFNGLGENBUFFERSPROC>("glGenBuffers");
		gl::bindBuffer = gl::LoadFunction<gl::PFNGLBINDBUFFERPROC>("glBindBuffer");
		gl::bufferData = gl::LoadFunction<gl::PFNGLBUFFERDATAPROC>("glBufferData");
		gl::deleteBuffers = gl::LoadFunction<gl::PFNGLDELETEBUFFERSPROC>("glDeleteBuffers");

		gl::createProgram = gl::LoadFunction<gl::PFNGLCREATEPROGRAMPROC>("glCreateProgram");
		gl::deleteProgram = gl::LoadFunction<gl::PFNGLDELETEPROGRAMPROC>("glDeleteProgram");
		gl::createShader = gl::LoadFunction<gl::PFNGLCREATESHADERPROC>("glCreateShader");
		gl::shaderSource = gl::LoadFunction<gl::PFNGLSHADERSOURCEPROC>("glShaderSource");
		gl::compileShader = gl::LoadFunction<gl::PFNGLCOMPILESHADERPROC>("glCompileShader");
		gl::getShaderiv = gl::LoadFunction<gl::PFNGLGETSHADERIVPROC>("glGetShaderiv");
		gl::attachShader = gl::LoadFunction<gl::PFNGLATTACHSHADERPROC>("glAttachShader");
		gl::linkProgram = gl::LoadFunction<gl::PFNGLLINKPROGRAMPROC>("glLinkProgram");
		gl::useProgram = gl::LoadFunction<gl::PFNGLUSEPROGRAMPROC>("glUseProgram");
		gl::detachShader = gl::LoadFunction<gl::PFNGLDETACHSHADERPROC>("glDetachShader");
		gl::deleteShader = gl::LoadFunction<gl::PFNGLDELETESHADERPROC>("glDeleteShader");
		gl::getShaderInfoLog = gl::LoadFunction<gl::PFNGLGETSHADERINFOLOGPROC>("glGetShaderInfoLog");
		gl::bindAttribLocation = gl::LoadFunction<gl::PFNGLBINDATTRIBLOCATIONPROC>("glBindAttribLocation");
		gl::uniformVector3fv = gl::LoadFunction<gl::PFNGLUNIFORM3FVPROC>("glUniform3fv");
		gl::uniformVector4fv = gl::LoadFunction<gl::PFNGLUNIFORM4FVPROC>("glUniform4fv");
		gl::uniformMatrix4fv = gl::LoadFunction<gl::PFNGLUNIFORMMATRIX4FVPROC>("glUniformMatrix4fv");
		gl::getUniformLocation = gl::LoadFunction<gl::PFNGLGETUNIFORMLOCATIONPROC>("glGetUniformLocation");

		gl::generateMipmap = gl::LoadFunction<gl::PFNGLGENERATEMIPMAPPROC>("glGenerateMipmap");

		std::cout << "Done" << std::endl;

		wglDeleteContext(gl_rc);
		DestroyWindow(handle);
#else

#endif
	}
	

}