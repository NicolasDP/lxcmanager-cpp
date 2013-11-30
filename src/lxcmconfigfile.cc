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
#include "lxcmjsonparser.hh"

LXCMConfigFile* LXCMConfigFile::_singleton = NULL;

LXCMConfigFile::LXCMConfigFile ()
  : LXCMCoreModule ("LXCMConfigFile")
{
  LXCMOptions* opts = LXCMOptions::getOptions ();

  opts->addModule (this);
  opts->addOption ("config,f", po::value<std::string> (),
                   "path to configuration file");
}

LXCMConfigFile::~LXCMConfigFile ()
{
  LXCMConfigFile::_singleton = NULL;
}

void LXCMConfigFile::init ()
{
  if (!LXCMConfigFile::_singleton)
  {
    LXCMConfigFile::_singleton = new LXCMConfigFile ();
  }
}

void LXCMConfigFile::checkOptions (po::variables_map& vm)
{
  if (vm.count ("config"))
  {
    std::string file (vm["config"].as<std::string> ());
    LXCMJsonParser tmp;

    tmp.parseFile (file);
  }
}

void __attribute__ ((constructor)) coremodule_init_configfile (void)
{
  LXCMConfigFile::init ();
}
