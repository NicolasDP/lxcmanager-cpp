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

#include "empty.hh"

LXCMPEmpty::LXCMPEmpty (PluginTools* pt)
  : LXCMPlugin ("plugin::Empty", 0x0001, pt)
  , _lockMessage (false)
{
}

LXCMPEmpty::~LXCMPEmpty ()
{
}

void LXCMPEmpty::init ()
{
  this->_pluginTools->log (LXCMLogger::DEBUG, "Initialize");
}

void LXCMPEmpty::start ()
{
  this->_pluginTools->log (LXCMLogger::DEBUG, "Start");
}

void LXCMPEmpty::stop ()
{
  this->_pluginTools->log (LXCMLogger::DEBUG, "Stop");
}

int LXCMPEmpty::receive (LXCMPlugin* from, std::string& message)
{
  int ret = -1;
  if (!this->_lockMessage)
  {
    this->_lockMessage = true;
    this->_pluginTools->log (LXCMLogger::DEBUG, "receive message '%s' from %s",
                           message.c_str (), from->moduleName ().c_str ());
    this->_lockMessage = false;
    ret = 0;
  }

  return ret;
}

void LXCMPEmpty::quit ()
{
  this->_pluginTools->log (LXCMLogger::DEBUG, "Quit");
}

extern "C" LXCMPlugin* create (PluginTools* pt)
{
  return new LXCMPEmpty (pt);
}
