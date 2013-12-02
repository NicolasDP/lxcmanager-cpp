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
#include "config.hh"
#include "plugintools.hh"
#include "exceptions.hh"
#include "lxcmconfigfile.hh"

#include <dirent.h>
#include <dlfcn.h>

/* The unique instance of the LXCMPlugModules Class */
LXCMPlugModules* LXCMPlugModules::_instance = NULL;

/* Constructor */
LXCMPlugModules::LXCMPlugModules ()
  : LXCMCoreModule ("LXCMPlugModules")
  , _libs ()
  , _modules ()
  , _modulesDirectory (CONFIG_CORE_DEFAULT_PLUGINS_DIRECTORY)
{
  LXCMOptions* opts = LXCMOptions::getOptions ();

  opts->addModule (this);
  opts->addOption ("plugdir,p", po::value<std::string> (),
                   "path to plugins directory");
}

LXCMPlugModules::~LXCMPlugModules ()
{
  /* TODO:
   *  -1- delete each _modules
   *  -2- close each _libs */
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
void LXCMPlugModules::checkOptions (po::variables_map& vm)
{
  if (vm.count ("plugdir"))
  {
    std::string directory (vm["plugdir"].as<std::string> ());

    log_message (LXCMLogger::DEBUG,
                 "plugin directory is '%s'", directory.c_str ());

    this->_modulesDirectory = directory;
  }
}

/* ----/ Plugin Loading /--------------------------------------------------- */

static PluginTools const plugTools =
{
  /* defined in include/logger.hh */
  .log = log_message,
  .send_message = send_message,
  .get_my_options = lxcm_configfile_get_my_config,
};

void LXCMPlugModules::exploreDir (std::string& dir)
{
  DIR* dp;
  struct dirent *ep;

  dp = opendir (dir.c_str ());

  if (dp)
  {
    while ((ep = readdir (dp)) != NULL)
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
          LXCMPlugin* (*create)(PluginTools const*);
          std::string name;
          LXCMPlugin* tmp = NULL;

          create = (LXCMPlugin* (*)(PluginTools const*))dlsym (lib, "create");
          if (create)
          {
            tmp = create (&plugTools);

            if (tmp)
            {
              name = tmp->moduleName ();
              log_message (LXCMLogger::INFO, "load module '%s'", name.c_str ());

              tmp->init ();

              this->_modules[name] = tmp;
              this->_libs[name] = lib;
            }
	  }
          else
          {
            dlclose (lib);
          }
        }
        break;
      case DT_DIR:
        /* If the iNode is a directory (but not the . nor the .. then explore
         * it */
        if (content.compare (".") && content.compare (".."))
        {
          try
          {
            this->exploreDir (path);
          }
          catch (LXCMException& e)
          {
            THROW_FORWARD_ERROR (e);
          }
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
    THROW_ERROR (errno, dir.c_str ());
  }
}

/* Main interface to request the PlugModules instance to load the modules */
void LXCMPlugModules::loadModules (void)
{
  LXCMPlugModules::_instance->exploreDir
                              (LXCMPlugModules::_instance->_modulesDirectory);
}

void LXCMPlugModules::sendMessageToPlugin (LXCMPlugin* from,
                                          std::string& to,
                                          std::string& message)
{
  LXCMPlugin* plugin = NULL;

  if (!from)
  {
    THROW_ERROR (EINVAL, "someone tries to send an anonymous message");
  }

  plugin = this->_modules[to];
  if (!plugin)
  {
    char message[256];
    memset (message, 0, sizeof (message));
    snprintf (message, sizeof (message),
              "%s tried to send a message to unknown module: %s",
              from->moduleName ().c_str (), to.c_str ());
    THROW_ERROR (EINVAL, message);
  }

  try
  {
    plugin->receive (from, message);
  }
  catch (LXCMException& e)
  {
    log_message (LXCMLogger::DEBUG, e.getErrorMessage ().c_str ());
    THROW_FORWARD_ERROR (e);
  }
}

void LXCMPlugModules::sendMessage (LXCMPlugin* from,
                                  char const* to,
                                  char const* message)
{
  std::string dest (to);
  std::string content (message);
  try
  {
    LXCMPlugModules::_instance->sendMessageToPlugin (from, dest, content);
  }
  catch (LXCMException& e)
  {
    THROW_FORWARD_ERROR (e);
  }
}

void send_message (LXCMPlugin* from, char const* to, char const* msg)
{
  LXCMPlugModules::sendMessage (from, to, msg);
}

/* ----/ Plugin Loading /--------------------------------------------------- */

void __attribute__ ((constructor)) coremodule_init_plugmodules (void)
{
  LXCMPlugModules::init ();
}
