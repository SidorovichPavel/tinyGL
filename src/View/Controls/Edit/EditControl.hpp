#pragma once
#include "../IControls.hpp"
#include "../../../Mesh/Mesh.hpp"

#include <string>

namespace tgl
{

	class EditControl : public IControls
	{
		std::string mText;
		Mesh mBackground;
		Mesh mWorkarea;

	public:
		EditControl();
		~EditControl();

		//overrided
		void set_focus() override;
		void kill_focus() override;

		void draw() override;
	private:
		void update() override;
	};

}