#include "ModuleLoader.h"

namespace Zia
{
	ModuleLoader::ModuleLoader(const std::string & basedir)
		: _basedir(basedir + '/')
	{
		DIR		*dir;
		if ((dir = opendir(this->_basedir.c_str())))
			closedir(dir);
		else
			throw (Zia::ModuleLoaderException("Cannot open modules basedir."));
	}

	ModuleLoader::~ModuleLoader()
	{
	}

	bool ModuleLoader::loadModule(const std::string & name)
	{
		ModuleDL		*mod = NULL;
		t_modInstance	modInst = NULL;
	
		if (this->_modules.count(name))
		{
			this->_logger.warning("Module \'" + name + "\' is already loaded.");
			return (false);
		}
		std::string pname = std::string(this->_basedir + name + LIB_EXT);
		try {
			mod = new ModuleDL(pname);
			modInst = (t_modInstance)mod->getSym("instantiate");
		} catch (ModuleDLException const &me) {
			delete (mod);
			throw (ModuleDLException(std::string(me.what()) + " for module \'" + name + "\'"));
		}
		this->_modules[name] = std::make_pair(mod, modInst());
		return (true);
	}

	bool ModuleLoader::unloadModule(const std::string & name)
	{
		if (this->_modules.count(name) < 1)
		{
			this->_logger.warning("Module \'" + name + "\' is not loaded but an attempt to unload it was made.");
			return (false);
		}
		delete (std::get<1>(this->_modules[name]));
		delete (std::get<0>(this->_modules[name]));
		this->_modules.erase(name);
		return (true);
	}

#ifdef linux

	ModuleDL::ModuleDL(const std::string &pname)
	{
		if (!(this->_hdl = dlopen(pname.c_str(), RTLD_LAZY)))
			throw (ModuleDLException("Failed to load library \'" + pname + "\'"));
	}

	void		*ModuleDL::getSym(const std::string &sym)
	{
		void	*mod;

		if (!(mod = dlsym(this->_hdl, sym.c_str())))
			throw (ModuleDLException("symbol \'" + sym + "\' not found"));
		return (mod);
	}

	ModuleDL::~ModuleDL()
	{
		dlclose(this->_hdl);
	}

#else
	
	ModuleDL::ModuleDL(const std::string &pname)
	{
		this->_hdl = LoadLibrary(pname.c_str());
		if (!this->hdl)
			throw (ModuleDLException("Failed to load library \'" + pname + "\'"));
	}

	void		*ModuleDL::getSym(const std::string &sym)
	{
		void	*mod;
		
		if (!(mod = GetProcAddress(sym, this->_hdl)))
			throw (ModuleDLException("symbol \'" + sym + "\' not found"));
		return (mod);
	}

	ModuleDL::~ModuleDL()
	{
		FreeLibrary(this->_hdl);
	}

#endif

}
