#include "View.h"
#include <stdexcept>

namespace tgl
{
	#ifdef _WIN32
		#include <gl/GL.h>
		#include <GL/glext.h>
	#endif
}


namespace tgl
{

#ifdef _WIN32
	using namespace win;

	LRESULT View::SWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
	{
		View* pWinView;
		if (uMsg == WM_NCCREATE)
		{
			pWinView = static_cast<View*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

			SetLastError(0);
			if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWinView)))
				if (GetLastError())
					return false;
		}
		else
			pWinView = reinterpret_cast<View*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		if (pWinView)
		{
			pWinView->mHandle = hWnd;
			return pWinView->WinProc(hWnd, uMsg, wParam, lParam);
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	bool View::is_open() noexcept
	{
		return isOpen;
	}

	void View::swap_buffers() noexcept
	{
		SwapBuffers(mDevice_content);
	}

	void View::init_opengl()
	{
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

		int pixel_format = ChoosePixelFormat(mDevice_content, &pfd);
		if (!pixel_format)
			throw std::runtime_error("tinyGL[Win32] -> feiled wile pixel format choose");

		if (!SetPixelFormat(mDevice_content, pixel_format, &pfd))
			throw std::runtime_error("tinyGL[Win32] -> failed wile set pixel format");

		mGL_resource_content = wglCreateContext(mDevice_content);
		wglMakeCurrent(mDevice_content, mGL_resource_content);
	}

	void View::create()
	{
	}

	void View::destroy()
	{
		this->isOpen = false;
		PostQuitMessage(EXIT_SUCCESS);
	}

	LRESULT View::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
	{
		switch (uMsg)
		{
		case WM_CREATE:
			create();
			break;
		case WM_DESTROY:
			destroy();
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
			break;
		}
		return 0;
	}

	View::View(const int width, const int height, const std::wstring& title, const Style& style)
	{
		WNDCLASSEX wc = { sizeof(wc) };
		wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpszClassName = title.c_str();
		wc.lpfnWndProc = View::SWinProc;

		if (!RegisterClassEx(&wc))
			throw std::runtime_error("class register error!");

		mHandle = CreateWindowEx(0, wc.lpszClassName, title.c_str(), WS_OVERLAPPEDWINDOW, 0, 0, width, height, 0, 0, 0, this);
		if (isOpen = static_cast<bool>(mHandle))
		{
			UpdateWindow(mHandle);
			ShowWindow(mHandle, SW_SHOWDEFAULT);
			mDevice_content = GetDC(mHandle);
		}
		else
			throw std::runtime_error("window create failed!");

		init_opengl();
	}

	View::View(View&& _Right) noexcept
	{
		std::swap(this->mHandle, _Right.mHandle);
		std::swap(this->isOpen, _Right.isOpen);
		std::swap(this->mDevice_content, _Right.mDevice_content);
		auto tpWinView = GetWindowLongPtr(this->mHandle, GWLP_USERDATA);
		auto rpWinView = GetWindowLongPtr(_Right.mHandle, GWLP_USERDATA);
		SetWindowLongPtr(this->mHandle, GWLP_USERDATA, rpWinView);
		SetWindowLongPtr(_Right.mHandle, GWLP_USERDATA, tpWinView);
	}

	View::~View()
	{
		wglDeleteContext(mGL_resource_content);
		if (isOpen)
			destroy();
	}
#elif 

#endif // _WIN32

}