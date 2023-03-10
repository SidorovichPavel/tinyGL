#pragma once

#include "ViewTypes.hpp"
#include "Detail.hpp"

namespace tgl {

	#ifdef _WIN32
		using View = win::WinHandler;
	#endif
}



