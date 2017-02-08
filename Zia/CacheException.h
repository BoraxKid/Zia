#pragma once

#include <stdexcept>
#include <exception>
#include <string>

namespace Zia
{
	class CacheException : public std::exception
    {
    public:
        CacheException(std::string what) throw() : _what(what) {}
        ~CacheException() {}

        virtual const char  *what() const throw() { return _what.c_str(); }

    private:
        std::string _what;
    };
}