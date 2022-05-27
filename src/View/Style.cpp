#include "Style.hpp"

namespace tgl
{
	Style::Style()
		:
		mXPos(0),
		mYPos(0),
		mWidth(0),
		mHeight(0)
	{
		mStates.reset();
	}

	Style::Style(string&& _Title,
		int _X, int _Y, int _W, int _H,
		bool _Centered, bool _Visible, bool _Fullscreen)
		:
		mXPos(_X),
		mYPos(_Y),
		mWidth(_W),
		mHeight(_H),
		mTitle(std::move(_Title))
	{
		mStates[static_cast<size_t>(State::Center)]		= _Centered;
		mStates[static_cast<size_t>(State::Visible)]	= _Centered;
		mStates[static_cast<size_t>(State::Fullscreen)] = _Centered;
	}

	Style::~Style() {};

	const string& Style::get_title() const noexcept
	{
		return mTitle;
	}

	void Style::set_title(const string& _Title) noexcept
	{
		mTitle = _Title;
	}

	void Style::set_state(State _State, bool _Val)
	{
		mStates[static_cast<size_t>(_State)] = _Val;
	}

	bool Style::get_state(State _State) const
	{
		return mStates[static_cast<size_t>(_State)];
	}

	std::pair<int, int> Style::get_size() const noexcept
	{
		return std::pair<int, int>(mWidth, mHeight);
	}

}