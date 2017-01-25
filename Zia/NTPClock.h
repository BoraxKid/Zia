#pragma once

#include <ctime>
namespace Zia
{
	class NTPClock
	{
	public:
		std::time_t getDate(void) const;
	};
}
