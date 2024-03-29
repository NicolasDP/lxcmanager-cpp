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

#include "version.hh"
#include "exceptions.hh"

LXCMVersion* LXCMVersion::_singleton = NULL;

LXCMVersion::LXCMVersion ()
  : LXCMCoreModule ("LXCMVersion")
{
  LXCMOptions* opts = LXCMOptions::getOptions ();

  opts->addModule (this);
  opts->addOption ("version", "print version");
}

LXCMVersion::~LXCMVersion ()
{
  LXCMVersion::_singleton = NULL;
}

void LXCMVersion::init ()
{
  if (!LXCMVersion::_singleton)
  {
    LXCMVersion::_singleton = new LXCMVersion ();
  }
}

void LXCMVersion::checkOptions (po::variables_map& vm)
{
  if (vm.count ("version"))
  {
    std::cout
<< CONFIG_PROJECT_NAME << " (" << CONFIG_PROJECT_VERSION << ") "
<< "Copyright (C) 2013 Nicolas DI PRIMA & Dorian ZACCARIA" << std::endl
<< "This program comes with ABSOLUTELY NO WARRANTY; for details read GPLv3."
   << std::endl
<< "This is free software, and you are welcome to redistribute it"
   << std::endl
<< "under certain conditions: see http://www.gnu.org/licenses/gpl.html."
   << std::endl;

    THROW_ERROR_CODE (0);
  }
}

void __attribute__ ((constructor)) coremodule_init_version (void)
{
  LXCMVersion::init ();
}
