#include "NTPClock.h"

std::time_t Zia::NTPClock::getDate(void) const
{
	return time(nullptr);
}
