#pragma once

#include <stdexcept>

namespace Zia
{
	class CacheException : public std::exception
	{
	public:
		CacheException(std::string const& what) : std::exception(what.c_str()) {}
	};
}