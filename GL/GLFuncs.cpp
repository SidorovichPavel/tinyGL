#include "GLFuncs.h"

namespace tgl::gl
{
	PFNGLGENBUFFERSPROC genBuffers = nullptr;
	PFNGLBINDBUFFERPROC bindBuffer = nullptr;
	PFNGLBUFFERDATAPROC bufferData = nullptr;
	PFNGLDELETEBUFFERSPROC deleteBuffers = nullptr;

	PFNGLGENVERTEXARRAYSPROC genVertexArrays = nullptr;
	PFNGLVERTEXATTRIBPOINTERPROC vertexArrtibPointer = nullptr;
	PFNGLBINDVERTEXARRAYPROC bindVertexArray = nullptr;
	PFNGLENABLEVERTEXATTRIBARRAYPROC enableVertexAttribArray = nullptr;
	PFNGLDISABLEVERTEXATTRIBARRAYPROC disableVertexAttribArray = nullptr;
	PFNGLDELETEVERTEXARRAYSPROC deleteVertexArrays = nullptr;

	PFNGLCREATEPROGRAMPROC createProgram = nullptr;
	PFNGLDELETEPROGRAMPROC deleteProgram = nullptr;
	PFNGLCREATESHADERPROC createShader = nullptr;
	PFNGLSHADERSOURCEPROC shaderSource = nullptr;
	PFNGLCOMPILESHADERPROC compileShader = nullptr;
	PFNGLGETSHADERIVPROC getShaderiv = nullptr;
	PFNGLATTACHSHADERPROC attachShader = nullptr;
	PFNGLLINKPROGRAMPROC linkProgram = nullptr;
	PFNGLUSEPROGRAMPROC useProgram = nullptr;
	PFNGLDETACHSHADERPROC detachShader = nullptr;
	PFNGLDELETESHADERPROC deleteShader = nullptr;
	PFNGLGETSHADERINFOLOGPROC getShaderInfoLog = nullptr;
	PFNGLBINDATTRIBLOCATIONPROC bindAttribLocation = nullptr;
	PFNGLGETUNIFORMLOCATIONPROC getUniformLocation = nullptr;
	PFNGLUNIFORMMATRIX4FVPROC uniformMatrix4fv = nullptr;
	PFNGLUNIFORM1IPROC uniform1i = nullptr;
}