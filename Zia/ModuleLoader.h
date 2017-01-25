#pragma once

#include <vector>


namespace Zia
{
	class ModuleLoader
	{
	public:
		ModuleLoader(const std::string &basedir);
		~ModuleLoader();
		bool	loadModule(const std::string &name);
		bool	unloadModule(const std::string &name);
	private:
		//std::vector<AModule *>	_modules;
	};
}