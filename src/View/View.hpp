﻿#pragma once

#include <memory>
#include <atomic>
#include <string>
#include "../style/style.hpp"
#include "Controls/IControls.hpp"
#include "../Event/Event.hpp"
#include "ViewTypes.hpp"
#include "Detail.hpp"

namespace tgl {
	
	template<class Handler>
	class FullView final : public Handler
	{
		using base = Handler;
		std::unique_ptr<Style> mStylePtr;
		std::vector<IControls*> mControls;
	public:
		FullView(Style* _StylePtr)
			:
			mStylePtr(_StylePtr),
			base(_StylePtr)
		{}
		FullView(const FullView& _Right) = delete;
		FullView(FullView&& _Right) = delete;
		~FullView() {};

		void enable_mouse_raw_input() { base::mouse_raw_input(); };
		void disable_mouse_raw_input() { base::mouse_raw_input(false); };
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

		float get_ratio() const noexcept
		{
			float den = base::mHeight == 0 ? 0.001f : static_cast<float>(base::mHeight);
			return static_cast<float>(base::mWidth) / den;
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


