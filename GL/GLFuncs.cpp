#include "GLFuncs.hpp"

namespace tgl::gl
{
	#define GL_FUNC_DECL(type, name) std::function<typename tgl_func<type>::func_t> name;

	GL_FUNC_DECL(PFNGLGENBUFFERSPROC, GenBuffers);
	GL_FUNC_DECL(PFNGLBINDBUFFERPROC, BindBuffer);
	GL_FUNC_DECL(PFNGLBUFFERDATAPROC, BufferData);
	GL_FUNC_DECL(PFNGLDELETEBUFFERSPROC, DeleteBuffers);

	GL_FUNC_DECL(PFNGLGENVERTEXARRAYSPROC, GenVertexArrays);
	GL_FUNC_DECL(PFNGLDELETEVERTEXARRAYSPROC, DeleteVertexArrays);
	GL_FUNC_DECL(PFNGLVERTEXATTRIBPOINTERPROC, VertexAttribPointer);
	GL_FUNC_DECL(PFNGLBINDVERTEXARRAYPROC, BindVertexArray);
	GL_FUNC_DECL(PFNGLENABLEVERTEXATTRIBARRAYPROC, EnableVertexAttribArray);
	GL_FUNC_DECL(PFNGLDISABLEVERTEXATTRIBARRAYPROC, DisableVertexAttribArray);

	GL_FUNC_DECL(PFNGLCREATEPROGRAMPROC, CreateProgram);
	GL_FUNC_DECL(PFNGLDELETEPROGRAMPROC, DeleteProgram);
	GL_FUNC_DECL(PFNGLCREATESHADERPROC, CreateShader);
	GL_FUNC_DECL(PFNGLSHADERSOURCEPROC, ShaderSource);
	GL_FUNC_DECL(PFNGLCOMPILESHADERPROC, CompileShader);
	GL_FUNC_DECL(PFNGLGETSHADERIVPROC, GetShaderiv);
	GL_FUNC_DECL(PFNGLATTACHSHADERPROC, AttachShader);
	GL_FUNC_DECL(PFNGLLINKPROGRAMPROC, LinkProgram);
	GL_FUNC_DECL(PFNGLUSEPROGRAMPROC, UseProgram);
	GL_FUNC_DECL(PFNGLDELETESHADERPROC, DeleteShader);
	GL_FUNC_DECL(PFNGLDETACHSHADERPROC, DetachShader);
	GL_FUNC_DECL(PFNGLGETSHADERINFOLOGPROC, GetShaderInfoLog);
	GL_FUNC_DECL(PFNGLBINDATTRIBLOCATIONPROC, BindAttribLocation);
	GL_FUNC_DECL(PFNGLGETUNIFORMLOCATIONPROC, GetUniformLocation);

	GL_FUNC_DECL(PFNGLUNIFORMMATRIX4FVPROC, UniformMatrix4fv);

	GL_FUNC_DECL(PFNGLUNIFORM4FVPROC, Uniform4fv);
	GL_FUNC_DECL(PFNGLUNIFORM3FVPROC, Uniform3fv);
	GL_FUNC_DECL(PFNGLUNIFORM4FPROC, Uniform4f);
	GL_FUNC_DECL(PFNGLUNIFORM1IPROC, Uniform1i);

	GL_FUNC_DECL(PFNGLACTIVETEXTUREPROC, ActiveTexture);
	GL_FUNC_DECL(PFNGLGENERATEMIPMAPPROC, GenerateMipmap);

	GL_FUNC_DECL(PFNGLDEBUGMESSAGECALLBACKPROC, DebugMessageCallback);
}