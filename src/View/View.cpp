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

	View::View(const int width, const int height, const std::wstring& title, const Style& style) :
		mWidth(width),
		mHeight(height)
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
		if (mIsOpen = static_cast<bool>(mHandle))
		{
			UpdateWindow(mHandle);
			ShowWindow(mHandle, SW_SHOWDEFAULT);
			mDevice_context = GetDC(mHandle);
		}
		else
			throw std::runtime_error("window create failed!");

		init_opengl();
	}
	
	View::View(View&& _Right) noexcept
	{
		mGL_resource_content = nullptr;
		std::swap(this->mHandle, _Right.mHandle);
		std::swap(this->mIsOpen, _Right.mIsOpen);
		std::swap(this->mDevice_context, _Right.mDevice_context);
		std::swap(this->mWidth, _Right.mWidth);
		std::swap(this->mHeight, _Right.mHeight);
		auto tpWinView = GetWindowLongPtr(this->mHandle, GWLP_USERDATA);
		auto rpWinView = GetWindowLongPtr(_Right.mHandle, GWLP_USERDATA);
		SetWindowLongPtr(this->mHandle, GWLP_USERDATA, rpWinView);
		SetWindowLongPtr(_Right.mHandle, GWLP_USERDATA, tpWinView);
	}
	
	View::~View()
	{
		wglDeleteContext(mGL_resource_content);
		if (mIsOpen)
			destroy();
	}

	LRESULT View::SWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
	{
		View* pWinView;
		if (uMsg == WM_NCCREATE)
		{
			pWinView = static_cast<View*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

			SetLastError(0);
			if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWinView)))
				if (GetLastError())
					return 0;
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

	LRESULT View::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
	{
		switch (uMsg)
		{
		case WM_CREATE:
			create();
			break;
		case WM_SIZE:
			size(static_cast<int>(LOWORD(lParam)), static_cast<int>(HIWORD(lParam)));
			break;
		case WM_DESTROY:
			destroy();
			this->mIsOpen = false;
			PostQuitMessage(EXIT_SUCCESS);
			break;
		case WM_KEYDOWN:
			key_down(static_cast<__int64>(wParam), static_cast<__int64>(lParam));
			break;
		case WM_MOUSEMOVE:
			mouse_move(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), static_cast<__int64>(wParam));
			break;
		case WM_MOVE:
			//move(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_MOVING:
			
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
			break;
		}
		return 0;
	}
	
	win::HWND View::get_handle() noexcept
	{
		return mHandle;
	}

	bool View::is_open() noexcept
	{
		return mIsOpen;
	}

	void View::swap_buffers() noexcept
	{
		SwapBuffers(mDevice_context);
	}

	void View::enale_opengl_context() noexcept
	{
		wglMakeCurrent(mDevice_context, mGL_resource_content);
		glViewport(0, 0, mWidth, mHeight);
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

		int pixel_format = ChoosePixelFormat(mDevice_context, &pfd);
		if (!pixel_format)
			throw std::runtime_error("tinyGL[Win32] -> feiled wile pixel format choose");

		if (!SetPixelFormat(mDevice_context, pixel_format, &pfd))
			throw std::runtime_error("tinyGL[Win32] -> failed wile set pixel format");

		mGL_resource_content = wglCreateContext(mDevice_context);
	}

	void View::create()	{}

	void View::destroy() {}

	void View::size(int width, int height) {}

	void View::key_down(__int64 virtual_code, __int64 state) {}

	void View::mouse_move(int x, int y, __int64 virtual_code) {}

	void View::moving(win::RECT* pRect){}

#elif 

#endif // _WIN32

}