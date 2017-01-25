#pragma once

#include <stdexcept>
#include <list>
#include <map>
#include "CachedData.h"
#include "NTPClock.h"

namespace Zia
{
#define DEFAULT_CACHE 0

	class CacheException : public std::exception
	{
	public:
		CacheException(std::string const& what) : std::exception(what.c_str()) {}
	};

	class CacheManager
	{
	private:
		unsigned long _nextID;
		NTPClock _clock;
		std::map<unsigned long, CachedData *> _cachedDataMap;
		unsigned long searchCachedData(apouche::HttpRequest const& request);
	public:
		CacheManager(void);
		~CacheManager(void);
		bool createCache(apouche::HttpRequest const& request, apouche::HttpResponse const& response, std::time_t expirationDate);
		bool updateCache(apouche::HttpRequest const & request, apouche::HttpResponse const & newResponse, std::time_t expirationTime);
		CachedData const& getCache(apouche::HttpRequest const& requset) const;
		bool deleteCacheData(apouche::HttpRequest const& request);
		apouche::HttpResponse getResponse(apouche::HttpRequest const& request);
		std::list<CachedData> * getDeprecatedCache(void) const;
	};
}