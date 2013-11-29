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
#include "exceptions.hh"

int main (int argc, char** argv)
{
  LXCMOptions* opts = LXCMOptions::getOptions ();

  try
  {
    opts->parseOptions (argc, argv);
  }
  catch (LXCMException& e)
  {
    if (e.getCode ())
    {
      log_message (LXCMLogger::ERROR, e.getErrorMessage ().c_str ());
      return e.getCode ();
    }
    return 0;
  }

  try
  {
    LXCMPlugModules::loadModules ();
  }
  catch (LXCMException& e)
  {
    log_message (LXCMLogger::ERROR, e.getErrorMessage ().c_str ());
    return e.getCode ();
  }

  return 0;
}
