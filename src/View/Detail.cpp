#include "Detail.h"
#include "..\Utility\utility.h"
#include <stdexcept>
#include <gl/GL.h>

#ifdef _WIN32
namespace tgl::win
{
	/***********************************************************************************************************************************
	****************************************************** WIN HANDLER *****************************************************************
	************************************************************************************************************************************/


	LRESULT WinHandler::GenProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
	{
		WinHandler* pWinH;
		if (uMsg == WM_NCCREATE)
		{
			pWinH = static_cast<WinHandler*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

			SetLastError(0);
			if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWinH)))
				if (GetLastError())
					return 0;
		}
		else
			pWinH = reinterpret_cast<WinHandler*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		if (pWinH)
		{
			pWinH->mHandle = hWnd;
			return pWinH->WinProc(hWnd, uMsg, wParam, lParam);
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	LRESULT WinHandler::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
	{
		unsigned int uiLParam = static_cast<unsigned int>(lParam);
		switch (uMsg)
		{
		case WM_CREATE:
			mEvents.create(hWnd);
			break;
		case WM_SIZE:
			GetWindowRect(mHandle, &mWinGlobalSize);
			mWidth = lo_word::get(uiLParam);
			mHeight = hi_word::get(uiLParam);
			mEvents.size(lo_word::get(uiLParam), hi_word::get(uiLParam));
			break;
		case WM_DESTROY:
			this->mIsOpen = false;
			PostQuitMessage(EXIT_SUCCESS);
			break;
		case WM_CLOSE:
			mEvents.close();
			DestroyWindow(mHandle);
			break;
		case WM_KEYDOWN:
			mEvents.key_down(static_cast<uint64_t>(wParam), static_cast<int64_t>(lParam));
			break;
		case WM_KEYUP:
			mEvents.key_up(static_cast<uint64_t>(wParam), static_cast<int64_t>(lParam));
			break;
		case WM_MOUSEMOVE:
			mEvents.mouse_move(get_x::get(uiLParam), get_y::get(uiLParam), static_cast<int64_t>(wParam));
			break;
		case WM_MOVE:
			GetWindowRect(mHandle, &mWinGlobalSize);

			mEvents.move(get_x::get(uiLParam), get_y::get(uiLParam));
			break;
		case WM_MOUSEWHEEL:
			mEvents.mouse_wheel(GET_KEYSTATE_WPARAM(wParam), GET_WHEEL_DELTA_WPARAM(wParam),
				get_x::get(uiLParam), get_y::get(uiLParam));
			break;
		case WM_LBUTTONDOWN:
			mEvents.mouse_lbutton_down(static_cast<int64_t>(wParam),
				get_x::get(uiLParam), get_y::get(uiLParam));
			break;
		case WM_LBUTTONUP:
			mEvents.mouse_lbutton_up(static_cast<int64_t>(wParam),
				get_x::get(uiLParam), get_y::get(uiLParam));
			break;
		case WM_RBUTTONDOWN:
			mEvents.mouse_rbutton_down(static_cast<int64_t>(wParam),
				get_x::get(uiLParam), get_y::get(uiLParam));
			break;
		case WM_RBUTTONUP:
			mEvents.mouse_rbutton_up(static_cast<int64_t>(wParam),
				get_x::get(uiLParam), get_y::get(uiLParam));
			break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			RECT Rect;

			GetClientRect(hWnd, &Rect);
			HDC hdc = BeginPaint(hWnd, &ps);

			// Создание теневого контекста для двойной буферизации
			HDC hCmpDC = CreateCompatibleDC(hdc);
			HBITMAP hBmp = CreateCompatibleBitmap(hdc, Rect.right - Rect.left,
				Rect.bottom - Rect.top);
			SelectObject(hCmpDC, hBmp);

			// Закраска фоновым цветом
			LOGBRUSH br;
			br.lbStyle = BS_SOLID;
			br.lbColor = 0xffffff;
			HBRUSH brush = CreateBrushIndirect(&br);
			FillRect(hCmpDC, &Rect, brush);
			DeleteObject(brush);

			// Здесь рисуем на контексте hCmpDC
			mEvents.paint(hCmpDC);

			// Копируем изображение из теневого контекста на экран
			SetStretchBltMode(hdc, COLORONCOLOR);
			BitBlt(hdc, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top,
				hCmpDC, 0, 0, SRCCOPY);

			// Удаляем ненужные системные объекты
			DeleteDC(hCmpDC);
			DeleteObject(hBmp);
			hCmpDC = NULL;

			EndPaint(hWnd, &ps);
		}
		break;
		case WM_INPUT:
		{
			mRawInputSize = 0;
			HRAWINPUT ri = (HRAWINPUT)lParam;

			GetRawInputData(ri, RID_INPUT, NULL, &mRawInputSize, sizeof(RAWINPUTHEADER));
			mRawInputData.resize(mRawInputSize);

			if (GetRawInputData(ri, RID_INPUT, mRawInputData.data(), &mRawInputSize, sizeof(RAWINPUTHEADER)) != mRawInputSize)
				break;

			RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(mRawInputData.data());

			mEvents.raw_input(raw);

			switch (raw->header.dwType)
			{
			case RIM_TYPEMOUSE:
				if (!mMouseRawInput)
					break;

				mEvents.mouse_raw_input(raw->data.mouse.usFlags, raw->data.mouse.lLastX, raw->data.mouse.lLastX);
				break;
			}
		}
		break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
			break;
		}
		return 0;
	}

	WinHandler::WinHandler(const int width, const int height, const std::string& title)
		:
		mWidth(width),
		mHeight(height),
		mGL_resource_content(0),
		mMouseRawInput(false),
		mRawInputHandle(0),
		mRawInputSize(0)
	{
		auto& temp = title;

		WNDCLASSEX wc = { sizeof(wc) };
		wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpszClassName = temp.c_str();
		wc.lpfnWndProc = WinHandler::GenProc;

		if (!RegisterClassEx(&wc))
			throw std::runtime_error("class register error!");

		mWinGlobalSize = { 0,0,width,height };
		AdjustWindowRect(&mWinGlobalSize, WS_OVERLAPPEDWINDOW, 0);
		mHandle = CreateWindowEx(
			0, wc.lpszClassName, temp.c_str(), WS_OVERLAPPEDWINDOW,
			0, 0, mWinGlobalSize.right - mWinGlobalSize.left, mWinGlobalSize.bottom - mWinGlobalSize.top,
			0, 0, 0,
			this);

		mIsOpen = static_cast<bool>(mHandle);
		if (mIsOpen)
		{
			UpdateWindow(mHandle);
			ShowWindow(mHandle, SW_SHOWDEFAULT);
			mDevice_context = GetDC(mHandle);
		}
		else
			throw std::runtime_error("window create failed!");

		mRawInputData.reserve(100);
	}

	WinHandler::~WinHandler()
	{
		if (mGL_resource_content)
			wglDeleteContext(mGL_resource_content);
		if (mIsOpen)
			DestroyWindow(mHandle);
	}

	void WinHandler::init_opengl()
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

	void WinHandler::enale_opengl_context() noexcept
	{
		wglMakeCurrent(mDevice_context, mGL_resource_content);
		glViewport(0, 0, mWidth, mHeight);
	}

	void WinHandler::mouse_raw_input(bool _Mode)
	{
		mMouseRawInput = true;
		if (_Mode)
		{
			RAWINPUTDEVICE rid = { 0x01,0x02,0,mHandle };

			if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
				throw std::runtime_error("failed while enable raw input device");
		}
		else
		{
			RAWINPUTDEVICE rid = { 0x01,0x02,RIDEV_REMOVE,nullptr };

			if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
				throw std::runtime_error("failed while disable raw input device");
		}
	}

	void WinHandler::show_cursor(bool mode) noexcept
	{
		if (!mode)
		{
			mouse_raw_input();
			ShowCursor(FALSE);
			RECT coords{ mWinGlobalSize.left + 50, mWinGlobalSize.top + 50,
				mWinGlobalSize.right - 50,mWinGlobalSize.bottom - 50 };
			ClipCursor(&coords);
		}
		else
		{
			ClipCursor(nullptr);
			ShowCursor(TRUE);
			mouse_raw_input(false);
		}
	}

	void WinHandler::send_message(uint32_t _Msg, uint64_t _WParam, int64_t _LParam) noexcept
	{
		win::SendMessage(mHandle, static_cast<UINT>(_Msg), static_cast<WPARAM>(_WParam), static_cast<LPARAM>(_LParam));
	}

	void WinHandler::post_message(uint32_t _Msg, uint64_t _WParam, int64_t _LParam) noexcept
	{
		win::PostMessage(mHandle, static_cast<UINT>(_Msg), static_cast<WPARAM>(_WParam), static_cast<LPARAM>(_LParam));
	}

	void WinHandler::invalidate_rect() noexcept
	{
		InvalidateRect(mHandle, nullptr, false);
	}

	void WinHandler::swap_buffers() noexcept
	{
		SwapBuffers(mDevice_context);
	}

	void WinHandler::destroy() noexcept
	{
		DestroyWindow(mHandle);
	}

	void WinHandler::close() noexcept
	{
		CloseWindow(mHandle);
	}

	detail::Events& WinHandler::events() noexcept
	{
		return mEvents;
	}

	/***********************************************************************************************************************************
	****************************************************** WIN MOUSE HANDLER ***********************************************************
	************************************************************************************************************************************/


}
#endif