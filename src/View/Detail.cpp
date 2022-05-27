#include <stdexcept>

#include "Detail.hpp"
#include <gl\GL.h>
#include "..\Utility\utility.hpp"

#ifdef _WIN32
namespace tgl::win
{
	/***********************************************************************************************************************************
	****************************************************** WIN HANDLER *****************************************************************
	************************************************************************************************************************************/

	LRESULT WinHandler::GeneralProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
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
		unsigned int lParam32 = static_cast<unsigned int>(lParam);
		unsigned long long wParam64 = static_cast<unsigned long long>(wParam);
		switch (uMsg)
		{
		case WM_CREATE:
			mEvents.create(hWnd);
			break;
		case WM_SIZE:
			GetWindowRect(mHandle, &mWinGlobalSize);
			mWidth = lo_word::get(lParam32);
			mHeight = hi_word::get(lParam32);
			mEvents.size(mWidth, mHeight);
			break;
		case WM_DESTROY:
			this->mIsOpen = false;
			PostQuitMessage(EXIT_SUCCESS);
			break;
		case WM_CLOSE:
			mEvents.close();
			DestroyWindow(mHandle);
			break;
		case WM_SETFOCUS:
			mEvents.set_focus();
			break;
		case WM_KILLFOCUS:
			mEvents.kill_focus();
			break;
		case WM_KEYDOWN:
			mEvents.key_down(wParam64, static_cast<int64_t>(lParam));
			break;
		case WM_KEYUP:
			mEvents.key_up(wParam64, static_cast<int64_t>(lParam));
			break;
		case WM_MOUSEMOVE:
			mEvents.mouse_move(cut_x::get(lParam32), cut_y::get(lParam32), wParam64);
			break;
		case WM_MOVE:
			GetWindowRect(mHandle, &mWinGlobalSize);
			mEvents.move(cut_x::get(lParam32), cut_y::get(lParam32));
			break;
		case WM_MOUSEWHEEL:
			mEvents.mouse_wheel(
				cut_key_state::get(wParam64),
				cut_wheel_delta::get(wParam64),
				cut_x::get(lParam32), cut_y::get(lParam32));
			break;
		case WM_LBUTTONDOWN:
			mEvents.mouse_lbutton_down(wParam64, cut_x::get(lParam32), cut_y::get(lParam32));
			break;
		case WM_LBUTTONUP:
			mEvents.mouse_lbutton_up(wParam64, cut_x::get(lParam32), cut_y::get(lParam32));
			break;
		case WM_RBUTTONDOWN:
			mEvents.mouse_rbutton_down(wParam64, cut_x::get(lParam32), cut_y::get(lParam32));
			break;
		case WM_RBUTTONUP:
			mEvents.mouse_rbutton_up(wParam64, cut_x::get(lParam32), cut_y::get(lParam32));
			break;
		case WM_PAINT:
			if (MDrawLib != DrawLib::Kernel32)
				break;

			GetClientRect(hWnd, &mClientRect);
			mDCHandle = BeginPaint(hWnd, &mPS);

			// Создание теневого контекста для двойной буферизации
			mCompatibleDCHandle = CreateCompatibleDC(mDCHandle);
			mCompatibleBitMapHandle = CreateCompatibleBitmap(mDCHandle, mClientRect.right - mClientRect.left,
				mClientRect.bottom - mClientRect.top);
			SelectObject(mCompatibleDCHandle, mCompatibleBitMapHandle);

			// Закраска фоновым цветом
			FillRect(mCompatibleDCHandle, &mClientRect, mBrushHandle);

			// Здесь рисуем на контексте hCmpDC
			mEvents.paint(mCompatibleDCHandle);

			// Копируем изображение из теневого контекста на экран
			SetStretchBltMode(mDCHandle, COLORONCOLOR);
			BitBlt(mDCHandle, 0, 0,
				mClientRect.right - mClientRect.left, mClientRect.bottom - mClientRect.top,
				mCompatibleDCHandle, 0, 0, SRCCOPY);

			// Удаляем ненужные системные объекты
			DeleteObject(mCompatibleBitMapHandle);
			DeleteDC(mCompatibleDCHandle);
			mCompatibleBitMapHandle = NULL;

			EndPaint(hWnd, &mPS);
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
				if (raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
					mEvents.mouse_move(raw->data.mouse.lLastX, raw->data.mouse.lLastY, 0);
				else
					mEvents.mouse_shift(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
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

	WinHandler::WinHandler(const Style* _Style_Ptr)
		:
		mGL_resource_content(0),
		mRawInputHandle(0),
		mRawInputSize(0),
		mPS({ 0 }),
		mClientRect({ 0 }),
		MDrawLib(DrawLib::Kernel32)
	{
		std::tie(mWidth, mHeight) = _Style_Ptr->get_size();
		auto [width, height] = _Style_Ptr->get_size();
		mWidth = width;
		mHeight = height;

		auto title = _Style_Ptr->get_title();

		WNDCLASSEX wc = { sizeof(wc) };
		wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpszClassName = title.c_str();
		wc.lpfnWndProc = WinHandler::GeneralProc;

		if (!RegisterClassEx(&wc))
			throw std::runtime_error("class register error!");

		mScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		mScreenHeight = GetSystemMetrics(SM_CYSCREEN);

		auto x = 0, y = 0;

		if (_Style_Ptr->get_state(Style::State::Center))
		{
			x = mScreenWidth - mWidth;
			x /= 2;
			y = mScreenHeight - mHeight;
			y /= 2;
		}

		mWinGlobalSize = { 0, 0, mWidth, mHeight };
		unsigned win_stylee = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME;
		AdjustWindowRect(&mWinGlobalSize, win_stylee, 0);
		mHandle = CreateWindowEx(
			0, wc.lpszClassName, title.c_str(), win_stylee,
			x, y,
			mWinGlobalSize.right - mWinGlobalSize.left, mWinGlobalSize.bottom - mWinGlobalSize.top,
			0, 0, 0,
			this);

		mIsOpen = static_cast<bool>(mHandle);
		if (mIsOpen && _Style_Ptr->get_state(Style::State::Visible))
		{
			UpdateWindow(mHandle);
			ShowWindow(mHandle, SW_SHOWDEFAULT);
			mDevice_context = GetDC(mHandle);
		}
		else
			throw std::runtime_error("window create failed!");

		//подготовка к рисованию через WM_PAINT
		GetClientRect(mHandle, &mClientRect);
		mDCHandle = BeginPaint(mHandle, &mPS);
		mCompatibleDCHandle = CreateCompatibleDC(mDCHandle);
		mCompatibleBitMapHandle = CreateCompatibleBitmap(mDCHandle,
			mClientRect.right - mClientRect.left,
			mClientRect.bottom - mClientRect.top);
		SelectObject(mCompatibleDCHandle, mCompatibleBitMapHandle);


		LOGBRUSH brush;
		brush.lbStyle = BS_SOLID;
		brush.lbColor = 0xffffffff;
		mBrushHandle = CreateBrushIndirect(&brush);

		mRawInputData.reserve(100);
	}

	WinHandler::~WinHandler()
	{
		DeleteObject(mBrushHandle);

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
			throw std::runtime_error("tinyGL : feiled wile pixel format choose");

		if (!SetPixelFormat(mDevice_context, pixel_format, &pfd))
			throw std::runtime_error("tinyGL : failed wile set pixel format");

		mGL_resource_content = wglCreateContext(mDevice_context);
		MDrawLib = DrawLib::OpenGL;
	}

	void WinHandler::enable_opengl_context() noexcept
	{
		wglMakeCurrent(mDevice_context, mGL_resource_content);
		glViewport(0, 0, mWidth, mHeight);
	}

	void WinHandler::mouse_raw_input(bool _Mode)
	{
		if (_Mode)
		{
			RAWINPUTDEVICE rid = { 0x01,0x02,0,mHandle };

			if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
				throw std::runtime_error("tinyGL : failed while enable raw input device");
		}
		else
		{
			RAWINPUTDEVICE rid = { 0x01,0x02,RIDEV_REMOVE,nullptr };

			if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
				throw std::runtime_error("tinyGL : failed while disable raw input device");
		}
	}

	void WinHandler::show_cursor(bool _Mode) noexcept
	{
		if (!_Mode)
		{
			mouse_raw_input(true);
			RECT coords{ mScreenWidth, mScreenHeight, mScreenWidth, mScreenHeight };
			ClipCursor(&coords);
		}
		else
		{
			ClipCursor(nullptr);
			SetCursorPos(mScreenWidth / 2, mScreenHeight / 2);
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

	void WinHandler::set_title(const std::string& title) noexcept
	{
		auto temp = std::wstring(title.begin(), title.end());
		win::SetWindowText(mHandle, temp.c_str());
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

	detail::Events& WinHandler::get_events() noexcept
	{
		return mEvents;
	}
}
#endif