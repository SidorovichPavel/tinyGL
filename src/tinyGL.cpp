#include "tinyGL.hpp"
#include "../GL/GLFuncs.hpp"

#include <stdexcept>
#include <iostream>

namespace tgl
{
	std::atomic<bool> opengl_is_init = false;

	detail::FrameTimeInfo FrameTimeInfo;
	std::pair<bool, int> event_pool(int fps, detail::FrameTimeInfo& fti) noexcept
	{
#ifdef _WIN32
		win::MSG msg{};

		using namespace std::chrono_literals;

		auto fix_particle = std::chrono::milliseconds(static_cast<int>(fps * 0.012));
		std::chrono::nanoseconds fps_lock = (1000ms / fps) + fix_particle;

		for (;;)
		{
			while (win::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				win::TranslateMessage(&msg);
				win::DispatchMessage(&msg);
			}

			auto current = std::chrono::steady_clock::now().time_since_epoch();
			std::chrono::nanoseconds wait;
			auto next_update = fti.next_update;

			if (current < next_update)
				wait = std::min(fps_lock, next_update - current);

			if (wait < 100ns)
			{
				fti.next_update = current + fps_lock;
				break;
			}

			auto msg_wait = std::chrono::duration_cast<std::chrono::milliseconds>(wait);

			if (win::MsgWaitForMultipleObjects(0, nullptr, FALSE, static_cast<uint32_t>(msg_wait.count()), QS_ALLEVENTS) == WAIT_TIMEOUT)
			{
				fti.next_update = std::chrono::steady_clock::now().time_since_epoch() + fps_lock;
				break;
			}
		}
		
		return std::make_pair(true, static_cast<int>(msg.wParam));
#else
		//TODO:
#endif
	}

	void Init()
	{
#ifdef _WIN32
		if (opengl_is_init)
			return;
		using namespace win;
		HWND handle = CreateWindowEx(0, TEXT("button"), TEXT(""), WS_POPUP, 0, 0, 0, 0, 0, 0, 0, 0);
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
			throw std::runtime_error("tinyGL::ChoosePixelFormat::feiled wile pixel format choose");

		if (!SetPixelFormat(dc, pixel_format, &pfd))
			throw std::runtime_error("tinyGL::SetPixelFormat::failed wile set pixel format");

		HGLRC gl_rc = wglCreateContext(dc);
		wglMakeCurrent(dc, gl_rc);

#define GL_INIT_FUNC(name, type, gl_name) name = gl::tgl_func<type>::LoadFunction(#gl_name)
#define GL_INIT(type, name) GL_INIT_FUNC(gl::name, gl::type, gl##name)

		GL_INIT(PFNGLGENBUFFERSPROC, GenBuffers);
		GL_INIT(PFNGLBINDBUFFERPROC, BindBuffer);
		GL_INIT(PFNGLBUFFERDATAPROC, BufferData);
		GL_INIT(PFNGLDELETEBUFFERSPROC, DeleteBuffers);

		GL_INIT(PFNGLGENVERTEXARRAYSPROC, GenVertexArrays);
		GL_INIT(PFNGLDELETEVERTEXARRAYSPROC, DeleteVertexArrays);
		GL_INIT(PFNGLVERTEXATTRIBPOINTERPROC, VertexAttribPointer);
		GL_INIT(PFNGLBINDVERTEXARRAYPROC, BindVertexArray);
		GL_INIT(PFNGLENABLEVERTEXATTRIBARRAYPROC, EnableVertexAttribArray);
		GL_INIT(PFNGLDISABLEVERTEXATTRIBARRAYPROC, DisableVertexAttribArray);

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
		GL_INIT(PFNGLUNIFORM4FPROC, Uniform4f);
		GL_INIT(PFNGLUNIFORM1IPROC, Uniform1i);

		GL_INIT(PFNGLACTIVETEXTUREPROC, ActiveTexture);
		GL_INIT(PFNGLGENERATEMIPMAPPROC, GenerateMipmap);

		GL_INIT(PFNGLDEBUGMESSAGECALLBACKPROC, DebugMessageCallback);

		wglDeleteContext(gl_rc);
		DestroyWindow(handle);

		opengl_is_init = true;
#else

#endif
	}

	void clear_black() noexcept
	{
		gl::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		tgl::gl::glClearColor(0.f, 0.f, 0.0f, 1.f);
	}

	void clear_color(float _R, float _G, float _B) noexcept
	{
		gl::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		tgl::gl::glClearColor(_R, _G, _B, 1.f);
	}

	void view_port(uint16_t _Width, uint16_t _Height)
	{
		gl::glViewport(0, 0, static_cast<unsigned int>(_Width), static_cast<unsigned int>(_Height));
	}

	void view_port_ex(int16_t _X, int16_t _Y, int16_t _Width, int16_t _Height)
	{
		gl::glViewport(_X, _Y, _Width, _Height);
	}

	namespace gl
	{
		void _stdcall callback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, char const* message, void const* user_param)
		{
			auto source_str = [source]() -> std::string {
				switch (source)
				{
				case GL_DEBUG_SOURCE_API: return "API";
				case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
				case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
				case GL_DEBUG_SOURCE_THIRD_PARTY:  return "THIRD PARTY";
				case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
				case GL_DEBUG_SOURCE_OTHER: return "OTHER";
				default: return "UNKNOWN";
				}
			}();

			auto type_str = [type]() {
				switch (type)
				{
				case GL_DEBUG_TYPE_ERROR: return "ERROR";
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
				case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
				case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
				case GL_DEBUG_TYPE_MARKER:  return "MARKER";
				case GL_DEBUG_TYPE_OTHER: return "OTHER";
				default: return "UNKNOWN";
				}
			}();

			auto severity_str = [severity]() {
				switch (severity) {
				case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
				case GL_DEBUG_SEVERITY_LOW: return "LOW";
				case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
				case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
				default: return "UNKNOWN";
				}
			}();

			std::cout << source_str << ", "
				<< type_str << ", "
				<< severity_str << ", "
				<< id << ": "
				<< message << std::endl;
		}




	}

}