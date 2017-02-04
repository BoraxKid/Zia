#include <time.h>
#include <algorithm>
#include "CachedData.h"

Zia::CachedData::CachedData(apouche::HttpRequest const & request, apouche::HttpResponse const & response, std::time_t const & _creationDate, std::time_t const & _expirationDate, unsigned long id)
	: _cachedID(id), _request(request), _response(response), _lastRefreshDate(_creationDate), _expirationDate(_expirationDate)
{
}

Zia::CachedData::~CachedData()
{
}

unsigned long Zia::CachedData::getCacheID(void) const
{
	return (this->_cachedID);
}

apouche::HttpRequest const & Zia::CachedData::getRequest(void) const
{
	return (this->_request);
}

void Zia::CachedData::setRequest(apouche::HttpRequest const & request)
{
	this->_request = request;
}

apouche::HttpResponse const & Zia::CachedData::getResponse(void) const
{
	return (this->_response);
}

void Zia::CachedData::setRespone(apouche::HttpResponse const & response, std::time_t updateTime, std::time_t expirationDate)
{
	this->_response = response;
	this->_lastRefreshDate = updateTime;
	this->_expirationDate = expirationDate;
}

std::time_t const & Zia::CachedData::getLastRefreshDate(void) const
{
	return (this->_lastRefreshDate);
}

void Zia::CachedData::setLastRefreshTime(std::time_t const & refreshTime)
{
	this->_lastRefreshDate = refreshTime;
}

std::time_t const & Zia::CachedData::getExpirationTime(void) const
{
	return (this->_expirationDate);
}

void Zia::CachedData::setExpirationTime(std::time_t expirationTime)
{
	this->_expirationDate = expirationTime;
}

std::time_t const & Zia::CachedData::getcurrentAge(std::time_t now) const
{
	std::time_t age = now - this->_lastRefreshDate;
	return (age > 0 ? age : 0);
}

std::time_t const & Zia::CachedData::getFreshnessLifetime(std::time_t now) const
{
	std::time_t freshnessLifetime = this->_expirationDate - now;
	return (freshnessLifetime > 0 ? freshnessLifetime : 0);
}

bool Zia::CachedData::isFresh(std::time_t now) const
{
	return (getFreshnessLifetime(now) > 0);
}