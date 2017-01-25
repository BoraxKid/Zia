#include "HeaderFactory.h"

namespace Zia
{
	HeaderFactory::HeaderFactory()
	{
	}


	HeaderFactory::~HeaderFactory()
	{
	}

	apouche::IHttpHeader *HeaderFactory::create(const std::string &key, const std::string &value)
	{
		apouche::IHttpHeader *header = new apouche::HttpHeader();
		header->setHeader(key, value);

		return (header);
	}


	void HeaderFactory::learn(const std::string &accept, apouche::IHttpHeader *header)
	{
		if (this->_headers.find(accept) == this->_headers.end())
		{
			this->_headers[accept] = header;
		}
		else
			std::cerr << "header already learned." << std::endl;
	}
}