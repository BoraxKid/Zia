#pragma once

#include <dirent.h>

#include <vector>
#include <string>
#include <map>
#include <fstream>

#ifdef linux

# define LIB_EXT	".so"
# include <dlfcn.h>
# define DL_HDL		void *

#else

# define LIB_EXT	".dll"
# include <windows.h>
# define DL_HDL		HMODULE

#endif

#include <Module/AModule.hpp>
#include <Logger/Logger.hh>

typedef apouche::AModule	*(*t_modInstance)();

namespace		Zia
{

	class ModuleDLException : public std::exception
	{
		public:
			ModuleDLException(std::string what) throw() : _what(what) {  }
			~ModuleDLException() {  }

        virtual const char  *what() const throw() { return _what.c_str(); }

		private:
			std::string _what;
	};

	class					ModuleDL
	{
		private:
			DL_HDL			_hdl;

		public:
			ModuleDL(const std::string &pname);
			~ModuleDL();
			void		*getSym(const std::string &sym);
	};

	class ModuleLoaderException : public std::exception
	{
		public:
			ModuleLoaderException(std::string what) throw() : _what(what) {  }
			~ModuleLoaderException() {  }

        virtual const char  *what() const throw() { return _what.c_str(); }

		private:
			std::string _what;
	};

	class		ModuleLoader
	{
	public:
		ModuleLoader(const std::string &basedir);
		~ModuleLoader();
		bool	loadModule(const std::string &name);
		bool	unloadModule(const std::string &name);

	private:
		std::map<std::string, std::pair<ModuleDL *, apouche::AModule *>> _modules;
		const std::string	_basedir;
		apouche::Logger		_logger;
		
	};

}
