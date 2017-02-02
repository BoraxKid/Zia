#ifndef ZIA_OPENSSLMODULE_HPP_
#define ZIA_OPENSSLMODULE_HPP_
#pragma once

#include "Module/AModule.hpp"

namespace Zia
{
    class OpenSSLModule : public apouche::AModule
    {
    public:
        OpenSSLModule(apouche::Logger *logger = new apouche::Logger());
        virtual ~OpenSSLModule();
        virtual void registerEvents(apouche::EventHandler *handler);

        bool beforeSendResponse(apouche::IHttpResponse *response, apouche::IHttpConf *conf);
    private:
        std::vector<std::string> _eventNames;
    };
}
#endif // ZIA_OPENSSLMODULE_HPP_
