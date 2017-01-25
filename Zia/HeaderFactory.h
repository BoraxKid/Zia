#pragma once

#include <iostream>
#include <map>

#include "../Submodule_Zia/HttpHeader/HttpHeader.hh"

namespace Zia {
	class HeaderFactory
	{
	public:
		HeaderFactory();
		~HeaderFactory();
		apouche::IHttpHeader *create(const std::string &key, const std::string &value);
		void learn(const std::string &accept, apouche::IHttpHeader *header);
	private:
		std::map<std::string, apouche::IHttpHeader*>	_headers;
	};
}