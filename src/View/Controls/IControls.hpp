#pragma once

namespace tgl
{
	namespace controls
	{
		namespace detail
		{

			class IControls
			{
				bool mNeedUpdate;
				void virtual update() = 0;
			public:
				IControls() = default;
				virtual ~IControls() = default;

				void virtual set_focus() = 0;
				void virtual kill_focus() = 0;

				void virtual draw() = 0;
			};

		}
	}
}