#pragma once

#include <string>
#include <bitset>

namespace tgl
{
	

	class Style
	{
		int mXPos, mYPos;
		int mWidth, mHeight;

		std::bitset<8> mStates;
		std::string mTitle;

	public:
		
		enum class State : size_t
		{
			Center = 0,
			Visible,
			Fullscreen
		};

		Style();
		Style(std::string&& _Title, 
			int _X = 0, int _Y = 0, int _W = 640, int _H = 480, 
			bool _Centered = true, bool _Visible = true, bool _Fullscreen = false);
		~Style();

		const std::string& get_title() const noexcept;
		void set_title(const std::string& _Title) noexcept;
		void set_state(State _State, bool _Val = true);
		bool get_state(State _State) const;
		std::pair<int, int> get_size() const noexcept;
	};

}