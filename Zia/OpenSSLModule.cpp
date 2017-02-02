#include "OpenSSLModule.hpp"

namespace Zia
{
    OpenSSLModule::OpenSSLModule(apouche::Logger *logger)
        : apouche::AModule("OpenSSL", "Encrypt and decrypt data", "0.0.1", logger)
    {
    }

    OpenSSLModule::~OpenSSLModule()
    {
    }

    void OpenSSLModule::registerEvents(apouche::EventHandler *handler)
    {
        this->_logger->info(this->_name + " v" + this->_version + ": Event registering");
        std::function<bool(apouche::IHttpResponse *, apouche::IHttpConf *)> function = std::bind(&OpenSSLModule::beforeSendResponse, this, std::placeholders::_1, std::placeholders::_2);
        this->_eventNames.push_back("Crypt request");
        apouche::Event<bool (apouche::IHttpResponse *, apouche::IHttpConf *)> event(this->_eventNames.at(0), apouche::Weight::HIGH, function);
        handler->_beforeSendResponse.addEvent(event);
    }

    bool OpenSSLModule::beforeSendResponse(apouche::IHttpResponse *response, apouche::IHttpConf *conf)
    {
        return (false);
    }
}
