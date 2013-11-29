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

#ifndef PLUGINTOOLS_HH_
# define PLUGINTOOLS_HH_

# include "logger.hh"

/*! @struct PluginsTools
 * @brief this structure will be given at the creation of each plugin in order
 * to give them an interface to communicate with the core (and also with all of
 * the plugins). */
typedef
struct pluginTools
{
  /*! @brief log is the methode given to the plugins to send log message properly
   * @param lvl is the log level
   * @param fmt is the string format
   * @param ... is the remaining argument to fill the format _fmt_ see (man 3
   * fprintf)*/
  void (*log) (LXCMLogger::level lvl, char const* fmt, ...);
}
PluginTools;


#endif /* !PLUGINTOOLS_HH_ */
