#pragma once

#include <deque>
#include <list>
#include <vector>
#include <string>
#include <array>
#include <iostream>

namespace tgl
{

	class flush_t {};
	extern flush_t flush;

	class Logger
	{
		std::list<std::string> mBuffer;
		std::ostream* mOut;
	public:
		Logger(std::ostream& _Out) noexcept;
		~Logger() noexcept;

		Logger& operator=(std::ostream& _Out) noexcept;

		Logger& operator<<(const std::string& _Str);
		Logger& operator<<(std::string&& _Str);
		void operator<<(const flush_t _Ph);

		template<class T>
		auto operator<<(const T _Val) ->
			std::enable_if_t<std::is_convertible_v<typename std::remove_cvref<T>::type, int>, Logger&>
		{
			mBuffer.push_back(std::to_string(_Val));
			return *this;
		}

		template<class T>
		auto operator<<(const T& _Obj) ->
			std::enable_if_t<
			std::is_same_v<decltype(std::declval<const T>().to_string()), std::string>,
			Logger&>
		{
			mBuffer.push_back(_Obj.to_string());
			return *this;
		}

	};

	extern Logger logger;

}