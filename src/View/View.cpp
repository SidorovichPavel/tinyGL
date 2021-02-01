#include "View.h"
#include <stdexcept>


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
			pWinView->handle = hWnd;
			return pWinView->WinProc(hWnd, uMsg, wParam, lParam);
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	bool View::is_open() noexcept
	{
		return isOpen;
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
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpszClassName = title.c_str();
		wc.lpfnWndProc = View::SWinProc;

		if (!RegisterClassEx(&wc))
			throw std::runtime_error("class register error!");

		handle = CreateWindowEx(0, wc.lpszClassName, title.c_str(), WS_OVERLAPPEDWINDOW, 0, 0, width, height, 0, 0, 0, this);
		if (isOpen = static_cast<bool>(handle))
		{
			UpdateWindow(handle);
			ShowWindow(handle, SW_SHOW);
		}
		else
			throw std::runtime_error("window create failed!");
	}

	View::View(View&& _Right) noexcept
	{
		std::swap(this->handle, _Right.handle);
		std::swap(this->isOpen, _Right.isOpen);
		auto tpWinView = GetWindowLongPtr(this->handle, GWLP_USERDATA);
		auto rpWinView = GetWindowLongPtr(_Right.handle, GWLP_USERDATA);
		SetWindowLongPtr(this->handle, GWLP_USERDATA, rpWinView);
		SetWindowLongPtr(_Right.handle, GWLP_USERDATA, tpWinView);
	}

	View::~View()
	{
		if (isOpen)
			destroy();
	}
#elif 

#endif // _WIN32

}