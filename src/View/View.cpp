#include <src/View/View.h>
#include <stdexcept>
#include <iostream>

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

		mWinGlobalSize = { 0,0,width,height };
		AdjustWindowRect(&mWinGlobalSize, WS_OVERLAPPEDWINDOW, 0);
		mHandle = CreateWindowEx(0, wc.lpszClassName, title.c_str(), WS_OVERLAPPEDWINDOW,
								 0, 0, mWinGlobalSize.right - mWinGlobalSize.left, mWinGlobalSize.bottom - mWinGlobalSize.top,
								 0, 0, 0, this);

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
		std::swap(mWinGlobalSize, _Right.mWinGlobalSize);
		std::swap(mGL_resource_content, _Right.mGL_resource_content);
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
			DestroyWindow(mHandle);
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
			create_event();
			break;
		case WM_SIZE:
			win::GetWindowRect(mHandle, &mWinGlobalSize);
			size_event(mWidth = LOWORD(lParam), mHeight = HIWORD(lParam));
			break;
		case WM_DESTROY:
			this->mIsOpen = false;
			PostQuitMessage(EXIT_SUCCESS);
			break;
		case WM_KEYDOWN:
			key_down_event(static_cast<__int64>(wParam), static_cast<__int64>(lParam));
			break;
		case WM_KEYUP:
			key_up_event(static_cast<__int64>(wParam), static_cast<__int64>(lParam));
			break;
		case WM_MOUSEMOVE:
			mouse_move_event(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), static_cast<__int64>(wParam));
			break;
		case WM_MOVE:
			win::GetWindowRect(mHandle, &mWinGlobalSize);
			move_event(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_MOUSEWHEEL:
			mouse_wheel_event(GET_KEYSTATE_WPARAM(wParam), GET_WHEEL_DELTA_WPARAM(wParam),
							  GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_INPUT:
		{
			UINT size = 0;
			HRAWINPUT ri = (HRAWINPUT)lParam;

			GetRawInputData(ri, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));
			uint8_t* data = new uint8_t[size];

			if (GetRawInputData(ri, RID_INPUT, data, &size, sizeof(RAWINPUTHEADER)) != size)
				break;
			
			RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(data);

			switch (raw->header.dwType)
			{
			case RIM_TYPEMOUSE:
				if (!mMouseRawInput)
					break;
			
				if (raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
					mVirtualMouse = std::make_pair(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				else
					mVirtualMouse += std::make_pair(raw->data.mouse.lLastX, raw->data.mouse.lLastY);

				mouse_raw_input_event(mVirtualMouse.dx(), mVirtualMouse.dy());
				break;
			}

			delete[] data;
		}
		break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
			break;
		}
		return 0;
	}

	void View::enable_mouse_raw_input()
	{
		mMouseRawInput = true;
		RAWINPUTDEVICE rid = { 0x01,0x02,0,mHandle };

		if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
			throw std::runtime_error("failed while enable raw input device");
	}

	void View::disable_mouse_raw_input()
	{
		mMouseRawInput = false;
		RAWINPUTDEVICE rid = { 0x01,0x02,RIDEV_REMOVE,nullptr };

		if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
			throw std::runtime_error("failed while disable raw input device");
	}

	win::HWND View::get_handle() noexcept
	{
		return mHandle;
	}

	void View::set_title(const std::wstring& title)
	{
		win::SetWindowText(mHandle, title.c_str());
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

	void View::destroy() noexcept
	{
		win::DestroyWindow(mHandle);
	}

	void View::show_cursor(bool mode)
	{
		win::ShowCursor(static_cast<int>(mode));
		if (!mode)
		{
			enable_mouse_raw_input();
			win::RECT coords{ mWinGlobalSize.left + 50, mWinGlobalSize.top + 50,
				mWinGlobalSize.right - 50,mWinGlobalSize.bottom - 50 };
			win::ClipCursor(&coords);
		}
		else
		{
			win::ClipCursor(nullptr);
			disable_mouse_raw_input();
		}
	}

	void View::center_cursour()
	{
		auto x = mWinGlobalSize.right + mWinGlobalSize.left;
		x /= 2;
		auto y = mWinGlobalSize.bottom + mWinGlobalSize.top;
		y /= 2;
		SetCursorPos(x, y);
	}

	std::pair<int, int> View::get_global_center()
	{
		auto x = mWinGlobalSize.right + mWinGlobalSize.left;
		x /= 2;
		auto y = mWinGlobalSize.bottom + mWinGlobalSize.top;
		y /= 2;
		return { x,y };
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

	#elif 

	#endif // _WIN32

}