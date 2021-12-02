#include "Style.h"

namespace tgl
{
	Style::Style()
		:
		x_pos(0),
		y_pos(0),
		width(0),
		height(0),
		centered(0),
		fullScreen(0),
		visible(0)
	{}
	Style::Style(std::string _Title, int _X, int _Y, int _W, int _H, unsigned _Centered, unsigned _Visible, unsigned _Fullscreen)
		:
		x_pos(_X),
		y_pos(_Y),
		width(_W),
		height(_H),
		centered(_Centered),
		visible(_Visible),
		fullScreen(_Fullscreen),
		title(std::move(_Title))
	{}



	Style::~Style() {};
}