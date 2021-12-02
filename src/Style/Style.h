#pragma once

#include <string>

#define gen(type, name)	private:\
							type name;\
						public:\
							type get_##name() const noexcept {return name;}\
							void set_##name(type _Value) noexcept {name = _Value;} 

namespace tgl
{

	class Style
	{
		gen(int, x_pos);
		gen(int, y_pos);
		gen(int, width);
		gen(int, height);
		gen(unsigned, centered);
		gen(unsigned, visible);
		gen(unsigned, fullScreen);

		std::string title;
	public:
		Style();
		Style(std::string _Title, int _X = 0, int _Y = 0, int _W = 640, int _H = 480, unsigned _Centered = 1, unsigned _Visible = 1, unsigned _Fullscreen = 0);
		~Style();

		const std::string& get_title() const noexcept
		{
			return title;
		}
		void set_title(const std::string& _Title) noexcept
		{
			title = _Title;
		}

	};

}


#undef gen
