#include <algorithm>
#include "CacheException.h"
#include "CacheManager.h"

unsigned long Zia::CacheManager::searchCachedData(apouche::HttpRequest const & request)
{
	std::map<unsigned long, CachedData *>::iterator it = std::find_if(this->_cachedDataMap.begin(), this->_cachedDataMap.end(), [request](std::pair<unsigned long, CachedData *> item)-> bool
	{
		return (item.second->getRequest().getHeaders() == request.getHeaders()
			&& item.second->getRequest().getBody() == request.getBody());
	});

	if (it != this->_cachedDataMap.end())
		return (it->first);
	return (DEFAULT_CACHE);
}

Zia::CacheManager::CacheManager(void)
{
	this->_nextID = 1;
}

Zia::CacheManager::~CacheManager(void)
{
	std::for_each(this->_cachedDataMap.begin(), this->_cachedDataMap.end(), [](std::pair<unsigned long, CachedData *> item)
	{
		delete(item.second);
	});
}

bool Zia::CacheManager::isEmpty(void) const
{
	return (this->_cachedDataMap.empty());
}

void Zia::CacheManager::addCache(CachedData * cacheData)
{
	if (cacheData == nullptr)
		throw (CacheException("CacheManager::addCache::cache is nullptr"));
	if (this->_cachedDataMap.find(cacheData->getCacheID()) != this->_cachedDataMap.end())
		throw (CacheException("CacheManager::addCache::already used cachedID:" + std::to_string(cacheData->getCacheID())));
	this->_cachedDataMap[cacheData->getCacheID()] = cacheData;
	this->_nextID = cacheData->getCacheID();
}

bool Zia::CacheManager::createCache(apouche::HttpRequest const & request, apouche::HttpResponse const & response, std::time_t expirationDate)
{
	//ça c'est caca
	std::find_if(this->_cachedDataMap.begin(), this->_cachedDataMap.end(), [request](std::pair<unsigned long, CachedData *> item)-> bool
	{
		if (request.getBody() == item.second->getRequest().getBody()
			&& request.getHeaders() == item.second->getRequest().getHeaders())
		{
			throw (CacheException("CacheManager::createCache::The given request is already cached"));
		}
		return (false);
	});
	CachedData * cachedData = new CachedData(request, response, this->_clock.getDate(), expirationDate, this->_nextID++);
	return (true);
}

bool Zia::CacheManager::updateCache(apouche::HttpRequest const & request, apouche::HttpResponse const & newResponse, std::time_t expirationTime)
{
	unsigned long cacheID = this->searchCachedData(request);
	if (cacheID == DEFAULT_CACHE)
		throw (CacheException("CacheManager::updateCache::The given request is not cached"));
	this->_cachedDataMap[cacheID]->setRespone(newResponse, this->_clock.getDate(), expirationTime);
	return (true);
}

Zia::CachedData const & Zia::CacheManager::getCache(apouche::HttpRequest const & request) const
{
	std::map<unsigned long, CachedData *>::const_iterator it = std::find_if(this->_cachedDataMap.begin(), this->_cachedDataMap.end(), [request](std::pair<unsigned long, CachedData *> item)-> bool
	{
		return (item.second->getRequest().getHeaders() == request.getHeaders()
			&& item.second->getRequest().getBody() == request.getBody());
	});

	if (it != this->_cachedDataMap.end())
		return (*it->second);
	throw (CacheException("CacheManager::getCache::The given request in not cached"));
}

bool Zia::CacheManager::deleteCacheData(apouche::HttpRequest const & request)
{
	unsigned long cacheID = this->searchCachedData(request);
	if (cacheID == DEFAULT_CACHE)
		throw (CacheException("CacheManager::updateCache::The given request is not cached"));
	this->_cachedDataMap.erase(cacheID);
	return (true);
}

apouche::HttpResponse Zia::CacheManager::getResponse(apouche::HttpRequest const & request)
{
	try
	{
		CachedData cache = this->getCache(request);
		return (cache.getResponse());
	}
	catch (CacheException e)
	{
		throw (CacheException("CacheManager::getResponse->" + std::string(e.what())));
	}
}

std::list<Zia::CachedData>* Zia::CacheManager::getDeprecatedCache(void) const
{
	std::list<CachedData> *list = new std::list<CachedData>();

	std::for_each(this->_cachedDataMap.begin(), this->_cachedDataMap.end(), [this, list](std::pair<unsigned long, CachedData*> item)
	{
		if (!item.second->isFresh(this->_clock.getDate()))
			list->push_back(*item.second);
	});
	return (list);
}
