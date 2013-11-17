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

#include "config.hh"
#include "options.hh"
#include "version.hh"
#include "modules.hh"
#include "logger.hh"

int main (int argc, char** argv)
{
  int ret;
  OptionsParseCode optPC;
  LXCMOptions* opts = LXCMOptions::getOptions ();
  LXCMLogger::init ();
  LXCMVersion::init ();
  LXCMPlugModules::init ();

  optPC = opts->parseOptions (argc, argv);

  LXCMLogger::log (LXCMLogger::INFO, "Program initialized");

  switch (optPC)
  {
    case ERR_NONE:
      ret = 0;
      break;
    case ERR_HELP:
      ret = 0;
      break;
    case ERR_ERROR:
    default:
      ret = 1;
      break;
  }

  LXCMLogger::log (LXCMLogger::INFO, "Program terminated (TODO)");

  return ret;
}
