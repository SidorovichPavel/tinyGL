#pragma once

#include <atomic>
#include <string>
#include <src/style/style.h>
#include <src/View/Mouse.h>
#include <src/View/Controls/IControls.h>
#include <src/Event/Event.h>
#include <src/View/ViewTypes.h>
#include <src/View/ViewDetail.h>

namespace tgl {
	#ifdef _WIN32
	namespace win
	{
		#define WIN32_LEAN_AND_MEAN
		#include <Windows.h>
		#include <windowsx.h>
		#include <WinUser.h>
	}

	template<class Handler>
	class View final : public Handler
	{
		using base = Handler;

		Mouse mVirtualMouse;
		std::vector<IControls*> mControls;
	public:
		View(const int width, const int height, const std::wstring& title)
			: base(width, height, title, mVirtualMouse)
		{}
		View(const View& _Right) = delete;
		View(View&& _Right) = delete;
		~View() {};

		void enable_mouse_raw_input() { base::mouse_raw_input(); };
		void disable_mouse_raw_input() { base::mouse_raw_input(false); };
		//void set_title(const std::wstring& title);
		//void center_cursour();
		//std::pair<int, int> get_global_center();
		bool is_open() noexcept
		{
			return base::mIsOpen;
		}

		//TODO: work with controls
		void add_control(IControls* _Control)
		{
			mControls.push_back(_Control);
		}
	};



	#endif



}



