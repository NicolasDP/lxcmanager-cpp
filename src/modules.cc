/* This file is part of LXCManager.
 *
 * LXCManager is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LXCManager is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LXCManager.  If not, see <http://www.gnu.org/licenses/>. */

#include "modules.hh"
#include "logger.hh"

#include <dirent.h>
#include <dlfcn.h>
LXCMPlugModules* LXCMPlugModules::_instance = NULL;

LXCMPlugModules::LXCMPlugModules ()
  : LXCMCoreModule ("LXCMPlugModules")
  , _modules ()
{
  LXCMOptions* opts = LXCMOptions::getOptions ();

  opts->addModule (this);
  opts->addOption ("plugdir,p", po::value<std::string> (),
                   "path to plugins directory");
}

void LXCMPlugModules::init (void)
{
  if (NULL == LXCMPlugModules::_instance)
  {
    LXCMPlugModules::_instance = new LXCMPlugModules ();
  }
}

OptionsParseCode LXCMPlugModules::checkOptions (po::variables_map& vm)
{
  if (vm.count ("plugdir"))
  {
    DIR* dp;
    struct dirent *ep;
    std::string directory (vm["plugdir"].as<std::string> ());

    std::cout << "Plugin directory is \""
              <<  directory << "\""
              <<  std::endl;

    dp = opendir (directory.c_str());
    if (dp)
    {
      while ((ep = readdir (dp)) != NULL)
      {
        std::string content;
	void* lib;
        switch (ep->d_type)
        {
        case DT_REG:
          content = ep->d_name;
          content = directory + std::string ("/") + content;
          lib = dlopen ( content.c_str (), RTLD_LAZY);
          if (lib)
	  {
	    std::cerr << "  " << content << " is a lib" << std::endl;
            LXCMPlugin* (*create)(void);
	    create = (LXCMPlugin* (*)(void))dlsym (lib, "create");
	    LXCMPlugin* tmp = create ();
	    std::string name (tmp->moduleName ());
	    std::cout << "    " << name << std::endl;
            _modules.insert (std::pair<std::string, LXCMPlugin*> (name, tmp));
	    /* TODO: not sure if this is here where I have to close the dlib */
	    dlclose (lib);
	  }
          break;
        default:
          break;
        }
      }
      closedir (dp);
    }
    else
    {
      LXCMLogger::log (LXCMLogger::ERROR, "Unable to find plugins");
      return ERR_ERROR;
    }
  }

  return ERR_NONE;
}

void __attribute__ ((constructor)) coremodule_init_plugmodules (void)
{
  LXCMPlugModules::init ();
}
