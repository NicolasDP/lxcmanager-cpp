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

#include "lxcmconfigfile.hh"
#include "exceptions.hh"
#include "logger.hh"
#include "lxcmjsonparser.hh"
#include <fstream>

LXCMConfigFile* LXCMConfigFile::_instance = NULL;

LXCMConfigFile::LXCMConfigFile ()
  : LXCMCoreModule ("core::LXCMConfigFile")
  , _dom (NULL)
{
  LXCMOptions* opts = LXCMOptions::getOptions ();

  opts->addModule (this);
  opts->addOption ("config,f", po::value<std::string> (),
                   "path to configuration file");
}

LXCMConfigFile::~LXCMConfigFile ()
{
  LXCMConfigFile::_instance = NULL;
}

void LXCMConfigFile::init ()
{
  if (!LXCMConfigFile::_instance)
  {
    LXCMConfigFile::_instance = new LXCMConfigFile ();
  }
}

void LXCMConfigFile::checkOptions (po::variables_map& vm)
{
  std::string filename (CONFIG_DEFAULT_CONFIGFILE);
  LXCMJsonParser tmp;
  std::ifstream ifs;
  char line_buffer[256];
  unsigned int line_counter = 0;

  if (vm.count ("config"))
  {
    filename = (vm["config"].as<std::string> ());
  }

  ifs.open (filename.c_str (), std::ifstream::in);

  do
  {
    memset (line_buffer, 0, sizeof (line_buffer));
    ifs.getline (line_buffer, sizeof (line_buffer) - 1);
    try
    {
      std::string theLine (line_buffer);
      tmp.parse (line_counter++, theLine);
    }
    catch (LXCMException& e)
    {
      THROW_FORWARD_ERROR (e);
    }
  }
  while (ifs.good ());

  this->_dom = tmp.getDom ();
#if defined (CONFIG_DEBUG) && CONFIG_DEBUG
  this->print (std::cout);
#endif
  ifs.close ();
}

LXCMJsonVal const* LXCMConfigFile::getModuleOptions (LXCMModule const* module)
{
  LXCMConfigFile const *instance = LXCMConfigFile::_instance;
  if (instance->_dom)
  {
    return instance->_dom->getObject (module->moduleName ());
  }

  return NULL;
}

void LXCMConfigFile::print (std::ostream& os) const
{
  if (this->_dom)
  {
    os << *this->_dom;
  }
}

LXCMJsonVal const* lxcm_configfile_get_my_config (LXCMModule const* module)
{
  return LXCMConfigFile::getModuleOptions (module);
}

std::ostream& operator<< (std::ostream& os, LXCMConfigFile const& obj)
{
  obj.print (os);
  return os;
}

static void __attribute__ ((constructor)) coremodule_init_configfile (void)
{
  LXCMConfigFile::init ();
}
