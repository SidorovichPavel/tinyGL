#pragma once

#include <memory>
#include <atomic>
#include <string>
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
		FullView(Style* _StylePtr) noexcept 
			:
			mStylePtr(_StylePtr),
			base(_StylePtr)
		{}
		~FullView()
		{};

		FullView(const FullView& _Right) = delete;
		FullView(FullView&& _Right) = delete;

		std::pair<int32_t, int32_t> get_size() noexcept
		{
			return std::pair<int32_t, int32_t>(base::mWidth, base::mHeight);
		}

		float get_ratio() noexcept
		{
			float den = base::mHeight == 0 ? 0.0001f : static_cast<float>(base::mHeight);
			return static_cast<float>(base::mWidth) / den;
		}

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


	#ifdef _WIN32
		using View = FullView<win::WinHandler>;
	#endif
}



