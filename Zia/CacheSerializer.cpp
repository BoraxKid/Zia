#include <fstream>
#include <sstream>
#include <algorithm>
#include "CacheSerializer.h"
#include "CacheException.h"
#include "NotImplementedException.h"

Zia::CacheSerializer::CacheSerializer(std::string const & cachePath)
	: _cachePath(cachePath)
{
	std::ifstream file(this->_cachePath + this->_cacheListFile);
	if (file.good())
	{
		std::string line;
		while (std::getline(file, line))
		{
			std::istringstream iss(line);
			unsigned long cacheID;
			if (!(iss >> cacheID))
				throw (CacheException("CacheSerializer::CacheSerializer::Unknown token:" + line));
			this->_cacheIDList.insert(cacheID);
		}
	}
}

bool Zia::CacheSerializer::checkCacheExistency(unsigned long cacheID)
{
	return (this->_cacheIDList.find(cacheID) != this->_cacheIDList.end());
}

void Zia::CacheSerializer::addCacheID(unsigned long cacheID)
{
	if (this->checkCacheExistency(cacheID))
		return;
	std::ofstream file(this->_cachePath + this->_cacheListFile, std::ios_base::app);
	file << std::to_string(cacheID);
}

std::string const & Zia::CacheSerializer::getCachePath(void) const
{
	return (this->_cachePath);
}

std::string const & Zia::CacheSerializer::getCacheListFileName(void) const
{
	return (this->_cacheListFile);
}

void Zia::CacheSerializer::populateCacheManage(CacheManager & cacheManager)
{

	if (!cacheManager.isEmpty())
		throw (CacheException("CacheSerializer::Cannot populate an unempty cacheManager"));
	std::for_each(this->_cacheIDList.begin(), this->_cacheIDList.end(), [this, cacheManager](unsigned long cacheID) mutable
	{
		StateModeHelper modeHelper;
		std::ifstream file(this->getCachePath() + std::to_string(cacheID));
		std::string line;

		stateMode mode;

		if (!file.good())
			throw (CacheException("CacheSerializer::populateCacheMaager::cannot acces cache file:" + std::to_string(cacheID)));

		CachedDataConstructor datas;
		datas._cachedID = cacheID;
		while (std::getline(file, line))
		{
			stateMode nextMode = modeHelper.getMode(line);
			mode = nextMode == noMode ? mode : nextMode;
			if (std::getline(file, line))
			{
				switch (mode)
				{
				case Zia::lastRefreshDate:
				{
					datas._lastRefreshDate = std::stoul(line.c_str());
					break;
				}
				case Zia::expirationDate:
				{
					datas._expirationDate = std::stoul(line.c_str());
					break;
				}
				case Zia::request:
				{
					std::pair<std::string, std::string> item;
					item.first = line.substr(0, line.find("<header_separator>"));
					item.second = line.substr(line.find("<header_separator>"));
					datas._requestMap[item.first] = item.second;
					break;
				}
				case Zia::requestBody:
				{
					datas._requestBody = line;
					break;
				}
				case Zia::response:
				{
					std::pair<std::string, std::string> item;
					item.first = line.substr(0, line.find("<header_separator>"));
					item.second = line.substr(line.find("<header_separator>"));
					datas._reponseMap[item.first] = item.second;
					break;
				}
				case Zia::responseBody:
				{
					datas._requestBody = line;
				}
				case Zia::noMode:
					break;
				default:
					break;
				}
			}
		}
		throw (NotImplementedException("CacheManager::PopulateCacheManager::Not Implemented yet because of a not complete API"));
	});

}

Zia::CacheSerializer & Zia::operator<<(CacheSerializer & oc, CachedData const & cache)
{
	//Append cache ID to the cacheIDListFile
	oc.addCacheID(cache.getCacheID());

	std::ofstream file(oc.getCachePath() + std::to_string(cache.getCacheID()), std::ios_base::trunc);
	//Cache the lifetime informations
	{
		file << "<lastRefreshDate>" << std::endl << std::to_string(cache.getLastRefreshDate()) << std::endl;
		file << "<ExpirationDate>" << std::endl << std::to_string(cache.getExpirationTime()) << std::endl;
	}
	//Cache the request
	{
		std::map<std::string, std::string> headerMap = cache.getRequest().getHeaders()->getAllHeader();
		file << "<request>" << std::endl;
		std::for_each(headerMap.begin(), headerMap.end(), [&file](std::pair<std::string, std::string> item)
		{
			file << item.first;
			file << "<header_separator>";
			file << item.second;
			file << std::endl;
		});
		file << "<request_body>" << std::endl << cache.getRequest().getBody()->getBody();
	}
	//Cache the response
	{
		std::map<std::string, std::string> headerMap = cache.getResponse().getHeaders()->getAllHeader();
		file << "<response>" << std::endl;
		std::for_each(headerMap.begin(), headerMap.end(), [&file](std::pair<std::string, std::string> item)
		{
			file << item.first;
			file << "<<header_separator>>";
			file << item.second;
			file << std::endl;
		});
		file << "<response_body>" << std::endl << cache.getResponse().getBody()->getBody();
	}
	file.close();
	return(oc);
}

std::string const & Zia::CachedDataConstructor::getMessage(stateMode mode) const
{
	std::string msg="";

	switch (mode)
	{
	case Zia::request:
	{
		std::for_each(this->_requestMap.begin(), this->_requestMap.end(), [msg](std::pair<std::string, std::string> item) mutable
		{
			msg += item.first + " " + item.second;
		});
		msg += "\r\n";
		msg += this->_requestBody;
		break;
	}
	case Zia::response:
	{
		std::for_each(this->_reponseMap.begin(), this->_reponseMap.end(), [msg](std::pair<std::string, std::string> item) mutable
		{
			msg += item.first + " " + item.second;
		});
		msg += "\r\n";
		msg += this->_responseBody;
		break;
	}
	default:
		break;
	}
	return (msg);
}
