#include "ViewBase.hpp"

namespace tgl
{
	namespace detail
	{

		ViewBase::ViewBase(int32_t _Width, int32_t _Height, int32_t _ScreenWidth, int32_t _ScreenHeight) noexcept
			:
			mWidth(_Width),
			mHeight(_Height),
			mScreenWidth(_ScreenWidth),
			mScreenHeight(_ScreenHeight),
			mIsOpen(false)
		{		}

		ViewBase::ViewBase() noexcept
			:
			mWidth(0),
			mHeight(0),
			mScreenWidth(0),
			mScreenHeight(0),
			mIsOpen(false)
		{		}

		ViewBase::~ViewBase()
		{		}

		std::pair<int32_t, int32_t> ViewBase::get_size() const noexcept
		{
			return std::make_pair(mWidth, mHeight);
		}

		float ViewBase::get_ratio() const noexcept
		{
			float den = mHeight == 0 ? 0.0001f : static_cast<float>(mHeight);
			return static_cast<float>(mWidth) / den;
		}

		bool ViewBase::is_open() const noexcept
		{
			return mIsOpen;
		}


	}
}