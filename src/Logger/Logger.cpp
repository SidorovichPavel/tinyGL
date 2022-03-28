#include "Logger.hpp"

#include <iostream>
#include <fstream>

namespace tgl
{
	flush_t flush;

	Logger::Logger(std::ostream& _Out) noexcept
		:
		mOut(&_Out)
	{
		*this << tgl::flush;
	}

	Logger::~Logger() noexcept
	{

	}

	Logger& Logger::operator=(std::ostream& _Out) noexcept
	{
		mOut = &_Out;
		return *this;
	}

	Logger& Logger::operator<<(const std::string& _Str)
	{
		mBuffer.push_back(_Str);
		return *this;
	}

	Logger& Logger::operator<<(std::string&& _Str)
	{
		mBuffer.push_back(std::move(_Str));
		return *this;
	}

	void Logger::operator<<(const flush_t _Ph)
	{
		for (auto&& str : mBuffer)
			*mOut << str << std::endl;
		*mOut << std::endl;
	}

	Logger logger = std::cout;
}