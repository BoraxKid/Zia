#pragma once

#include <stdexcept>

namespace Zia
{
	class NotImplementedException : public std::logic_error
	{
	public:
		NotImplementedException(std::string const& what) :
			std::logic_error(what.c_str())
		{}
	};
}