#pragma once

#include <bitset>
#include "..\utility\utility.hpp"

namespace tgl
{
	#ifdef _WIN32
	namespace win
	{
		enum class WinModifier : uint32_t
		{
			Border = 0x00800000u,
			Caption = 0x00C00000u,
			Child = 0x40000000u,
			ChildWindow = 0x40000000u,
			ClipChldren = 0x02000000u,
			ClipSiblings = 0x04000000u,
			Disabled = 0x08000000u,
			DlgFrame = 0x00400000u,
			Group = 0x00020000u,
			HScroll = 0x00100000u,
			Iconic = 0x20000000u,
			Maximaze = 0x01000000u,
			MaximazeBox = 0x00010000u,
			Minimaze = 0x20000000u,
			MinimazeBox = 0x00020000u,
			Overlapped = 0x00000000u,
			ThickFrame = 0x00040000u,
			SysMenu = 0x00080000u,
			OverlappedWindow = Overlapped | Caption | SysMenu | ThickFrame | MinimazeBox | MaximazeBox,
			PopUp = 0x80000000u,
			PopUpWindow = PopUp | Border | SysMenu,
			SizeBox = 0x00040000u,
			TabStop = 0x00010000u,
			Tiled = 0x00000000u,
			TiledWindow = OverlappedWindow,
			Visible = 0x10000000u,
			VScroll = 0x00200000u
		};
	}
	#endif

	using StyleModifier = win::WinModifier;

	class Style
	{
		int mXPos, mYPos;
		int mWidth, mHeight;

		std::bitset<8> mStates;
		string mTitle;

		uint32_t mCompressedStyle;
	public:



		enum class State : size_t
		{
			Center = 0,
			Visible,
			Fullscreen
		};

		Style();
		Style(string&& _Title,
			int _X = 0, int _Y = 0, int _W = 640, int _H = 480,
			bool _Centered = true, bool _Visible = true, bool _Fullscreen = false);
		~Style();

		const string& get_title() const noexcept;
		void set_title(const string& _Title) noexcept;
		void set_state(State _State, bool _Val = true);
		bool get_state(State _State) const;
		std::pair<int, int> get_size() const noexcept;

		Style& operator<<(StyleModifier _Mod) noexcept;
		Style& operator>>(StyleModifier _Mod) noexcept;
		uint32_t get_modifier() const noexcept;
	};

}