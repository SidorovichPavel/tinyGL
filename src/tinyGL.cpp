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

		#define GL_INIT_FUNC(name, type, gl_name) name = gl::tgl_func<type>::LoadFunction(#gl_name)
		#define GL_INIT(type, name) GL_INIT_FUNC(gl::name, gl::type, gl##name)

		GL_INIT(PFNGLGENBUFFERSPROC, GenBuffers);
		GL_INIT(PFNGLBINDBUFFERPROC, BindBuffer);
		GL_INIT(PFNGLBUFFERDATAPROC, BufferData);
		GL_INIT(PFNGLDELETEBUFFERSPROC, DeleteBuffers);

		GL_INIT(PFNGLGENVERTEXARRAYSPROC, GenVertexArrays);
		GL_INIT(PFNGLVERTEXATTRIBPOINTERPROC, VertexAttribPointer);
		GL_INIT(PFNGLBINDVERTEXARRAYPROC, BindVertexArray);
		GL_INIT(PFNGLENABLEVERTEXATTRIBARRAYPROC, EnableVertexAttribArray);
		GL_INIT(PFNGLDISABLEVERTEXATTRIBARRAYPROC, DisableVertexAttribArray);
		GL_INIT(PFNGLDELETEVERTEXARRAYSPROC, DeleteVertexArrays);

		GL_INIT(PFNGLCREATEPROGRAMPROC, CreateProgram);
		GL_INIT(PFNGLDELETEPROGRAMPROC, DeleteProgram);
		GL_INIT(PFNGLCREATESHADERPROC, CreateShader);
		GL_INIT(PFNGLSHADERSOURCEPROC, ShaderSource);
		GL_INIT(PFNGLCOMPILESHADERPROC, CompileShader);
		GL_INIT(PFNGLGETSHADERIVPROC, GetShaderiv);
		GL_INIT(PFNGLATTACHSHADERPROC, AttachShader);
		GL_INIT(PFNGLLINKPROGRAMPROC, LinkProgram);
		GL_INIT(PFNGLUSEPROGRAMPROC, UseProgram);
		GL_INIT(PFNGLDELETESHADERPROC, DeleteShader);
		GL_INIT(PFNGLDETACHSHADERPROC, DetachShader);
		GL_INIT(PFNGLGETSHADERINFOLOGPROC, GetShaderInfoLog);
		GL_INIT(PFNGLBINDATTRIBLOCATIONPROC, BindAttribLocation);
		GL_INIT(PFNGLGETUNIFORMLOCATIONPROC, GetUniformLocation);

		GL_INIT(PFNGLUNIFORMMATRIX4FVPROC, UniformMatrix4fv);
		GL_INIT(PFNGLUNIFORM4FVPROC, Uniform4fv);
		GL_INIT(PFNGLUNIFORM3FVPROC, Uniform3fv);
		GL_INIT(PFNGLUNIFORM1IPROC, Uniform1i);

		GL_INIT(PFNGLACTIVETEXTUREPROC, ActiveTexture);
		GL_INIT(PFNGLGENERATEMIPMAPPROC, GenerateMipmap);

		GL_INIT(PFNGLDEBUGMESSAGECALLBACKPROC, DebugMessageCallback);
		
		std::cout << "Done" << std::endl;

		wglDeleteContext(gl_rc);
		DestroyWindow(handle);
#else

#endif
	}
	

}