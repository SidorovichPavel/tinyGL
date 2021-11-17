#pragma once

#include <atomic>
#include <string>
#include <src/style/style.h>
#include <src/View/Controls/IControls.h>
#include <src/Event/Event.h>
#include <src/View/ViewTypes.h>
#include <src/View/Detail.h>

namespace tgl {
	
	template<class Handler>
	class FullView final : public Handler
	{
		using base = Handler;

		std::vector<IControls*> mControls;
	public:
		FullView(const int width, const int height, const std::string& title)
			: base(width, height, title)
		{}
		FullView(const FullView& _Right) = delete;
		FullView(FullView&& _Right) = delete;
		~FullView() {};

		void enable_mouse_raw_input() { base::mouse_raw_input(); };
		void disable_mouse_raw_input() { base::mouse_raw_input(false); };
		//void set_title(const std::wstring& title);
		//void center_cursour();
		//std::pair<int, int> get_global_center();
		bool is_open() noexcept
		{
			return base::mIsOpen;
		}

		std::pair<int, int> get_size() const noexcept
		{
			return std::pair<int, int>(base::mWidth, base::mHeight);
		}
		//TODO: work with controls
		void add_control(IControls* _Control)
		{
			mControls.push_back(_Control);
		}
	};


	#ifdef _WIN32
		using View = FullView<win::WinHandler>;
	#endif
}



