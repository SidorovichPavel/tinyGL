#include "Button.hpp"

namespace tgl
{
	namespace controls
	{
		namespace shader
		{
			std::string basic[3]
			{
				R"(#version 330 
				location(layout = 0) in vec3 vPosition;
				
				void main()
				{
					gl_Position = vPosition;
				}
				)",
				R"(#version 330
				out vec4 resColor;

				void main()
				{
					resColor = vec4(.5f,.5f,.5f,1.f);
				}
				)",
				""
			};
		}



	}
}