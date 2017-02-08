#pragma once
#include <ctime>
#include "HttpRequest/HttpRequest.hh"
#include "HttpResponse/HttpResponse.hh"

namespace Zia
{
	class CachedData
	{
	private:
		unsigned long const		_cachedID;
		apouche::HttpRequest	_request;
		apouche::HttpResponse	_response;
		std::time_t				_lastRefreshDate;
		std::time_t				_expirationDate;
		
	public:
		CachedData(apouche::HttpRequest const& request, apouche::HttpResponse const& response,
					std::time_t const& _creationDate = 0, std::time_t const& _expirationDate = 0, unsigned long id = 0);
		~CachedData();

		unsigned long getCacheID(void) const;
		apouche::HttpRequest const& getRequest(void) const;
		void setRequest(apouche::HttpRequest const& request);
		apouche::HttpResponse const& getResponse(void) const;
		void setRespone(apouche::HttpResponse const & response, std::time_t updateTime, std::time_t expirationDate);
		std::time_t const& getLastRefreshDate(void) const;
		void setLastRefreshTime(std::time_t const& refreshTime);
		std::time_t const& getExpirationTime(void) const;
		void setExpirationTime(std::time_t expirationTime);
		std::time_t const& getcurrentAge(std::time_t now) const;
		std::time_t const& getFreshnessLifetime(std::time_t now) const;
		bool isFresh(std::time_t now) const;		
	};
}

