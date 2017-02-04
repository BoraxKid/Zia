#pragma once
#include <string>
#include <set>
#include "CachedData.h"
#include "CacheManager.h"

namespace Zia
{
#define LRFMODE "<lastRefreshDate>"
#define EXPMODE "<ExpirationDate>"
#define REQMODE "<request>"
#define RQBMODE "<request_body>"
#define RPBMODE "<response_body>"
#define RESMODE "<response>"

	enum stateMode
	{
		lastRefreshDate,
		expirationDate,
		request,
		requestBody,
		response,
		responseBody,
		noMode
	};

	class StateModeHelper
	{
	private:
		std::map<std::string, stateMode> _modeMap;
	public:
		StateModeHelper()
		{
			this->_modeMap[LRFMODE] = lastRefreshDate;
			this->_modeMap[EXPMODE] = expirationDate;
			this->_modeMap[REQMODE] = request;
			this->_modeMap[RQBMODE] = requestBody;
			this->_modeMap[RESMODE] = response;
			this->_modeMap[RPBMODE] = responseBody;

		}

		stateMode getMode(std::string const& querry) const
		{
			if (this->_modeMap.find(querry) != this->_modeMap.end())
				return (this->_modeMap.find(querry)->second);
			return (noMode);
		}
	};

	class CachedDataConstructor
	{
	public:
		unsigned long						_cachedID;
		std::map<std::string, std::string>	_requestMap;
		std::string							_requestBody;
		std::map<std::string, std::string>	_reponseMap;
		std::string							_responseBody;
		std::time_t							_lastRefreshDate = 0;
		std::time_t							_expirationDate = 0;

		std::string const& getMessage(stateMode mode) const;
	};

	class CacheSerializer
	{
	private:
		std::string const& _cacheListFile = "cacheList";
		std::string const& _cachePath;
		std::set<unsigned long> _cacheIDList;
	public:
		CacheSerializer(std::string const& cachePath = "Cache");
		bool checkCacheExistency(unsigned long cacheID);
		void addCacheID(unsigned long cacheID);
		std::string const& getCachePath(void) const;
		std::string const& getCacheListFileName(void) const;
		void populateCacheManage(CacheManager & cacheManager);
	};
	 
	CacheSerializer & operator<<(CacheSerializer & oc, CachedData const& cache);
}