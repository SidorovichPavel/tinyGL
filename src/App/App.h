#pragma once
#include <functional>
namespace tgl
{

#ifdef _WIN32
	namespace win
	{
#include<Windows.h>
	}
#endif



	class IUserData
	{
	public:
		IUserData();
		virtual ~IUserData();
		void virtual call() = 0;
	};

	class App
	{
	public:
		static void Init(int argn, char** argc);
		static int Run(std::function<void(void)> _Fn);
	};
}