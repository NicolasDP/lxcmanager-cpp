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
#include "config.hh"

#include <dirent.h>
#include <dlfcn.h>

/* The unique instance of the LXCMPlugModules Class */
LXCMPlugModules* LXCMPlugModules::_instance = NULL;

/* Constructor */
LXCMPlugModules::LXCMPlugModules ()
  : LXCMCoreModule ("LXCMPlugModules")
  , _modules ()
  , _modulesDirectory (CONFIG_CORE_DEFAULT_PLUGINS_DIRECTORY)
{
  LXCMOptions* opts = LXCMOptions::getOptions ();

  opts->addModule (this);
  opts->addOption ("plugdir,p", po::value<std::string> (),
                   "path to plugins directory");
}

/* Initialize the unique instance if needed */
void LXCMPlugModules::init (void)
{
  if (NULL == LXCMPlugModules::_instance)
  {
    LXCMPlugModules::_instance = new LXCMPlugModules ();
  }
}

/* Check the related options and handle them */
OptionsParseCode LXCMPlugModules::checkOptions (po::variables_map& vm)
{
  if (vm.count ("plugdir"))
  {
    std::string directory (vm["plugdir"].as<std::string> ());

    log_message (LXCMLogger::DEBUG,
                 "plugin directory is '%s'", directory.c_str ());

    this->_modulesDirectory = directory;
  }

  return ERR_NONE;
}

/* ----/ Plugin Loading /--------------------------------------------------- */

int LXCMPlugModules::exploreDir (std::string& dir)
{
  DIR* dp;
  struct dirent *ep;
  int ret = 0;

  dp = opendir (dir.c_str ());

  if (dp)
  {
    while ((!ret) && (ep = readdir (dp)) != NULL)
    {
      std::string content (ep->d_name);
      std::string path (dir + std::string ("/") + content);
      void* lib;

      switch (ep->d_type)
      {
      case DT_REG:
        /* If the iNode is a regular file then, try to load it, if it fails,
         * then it's not a module, else load it, create the instance and save
         * it inside the module list instance (_modules) */
        lib = dlopen ( path.c_str (), RTLD_LAZY);
        if (lib)
        {
          LXCMPlugin* (*create)(void);
          std::string name;

          create = (LXCMPlugin* (*)(void))dlsym (lib, "create");
          LXCMPlugin* tmp = create ();

          name = tmp->moduleName ();
          log_message (LXCMLogger::INFO, "load module '%s'", name.c_str ());

          this->_modules.insert (std::pair<std::string, LXCMPlugin*> (name, tmp));

          /* TODO: not sure if this is here where I have to close the dlib */
          dlclose (lib);
        }
        break;
      case DT_DIR:
        /* If the iNode is a directory (but not the . nor the .. then explore
         * it */
        if (content.compare (".") && content.compare (".."))
        {
          ret = this->exploreDir (path);
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
    log_message (LXCMLogger::ERROR, "can't open directory: %s", dir.c_str ());
    ret = -1;
  }

  return ret;
}

/* Main interface to request the PlugModules instance to load the modules */
int LXCMPlugModules::loadModules (void)
{
  return LXCMPlugModules::_instance->exploreDir
                          (LXCMPlugModules::_instance->_modulesDirectory);
}

/* ----/ Plugin Loading /--------------------------------------------------- */

void __attribute__ ((constructor)) coremodule_init_plugmodules (void)
{
  LXCMPlugModules::init ();
}
