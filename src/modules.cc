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

LXCMPlugModules* LXCMPlugModules::_modules = NULL;

LXCMPlugModules::LXCMPlugModules ()
  : LXCMCoreModule ("LXCMPlugModules")
{
  LXCMOptions* opts = LXCMOptions::getOptions ();

  opts->addModule (this);
  opts->addOption ("plugdir,p", po::value<std::string> (),
                   "path to plugins directory");
}

void LXCMPlugModules::init (void)
{
  if (NULL == LXCMPlugModules::_modules)
  {
    LXCMPlugModules::_modules = new LXCMPlugModules ();
  }
}

OptionsParseCode LXCMPlugModules::checkOptions (po::variables_map& vm)
{
  if (vm.count ("plugdir"))
  {
    std::cout << "Plugin directory is \""
              <<  vm["plugdir"].as<std::string> () << "\""
              <<  std::endl;
  }

  return ERR_NONE;
}

void __attribute__ ((constructor)) coremodule_init_plugmodules (void)
{
  LXCMPlugModules::init ();
}
