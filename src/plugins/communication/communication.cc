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

#include "communication.hh"

LXCMPCommunication::LXCMPCommunication (PluginTools* pt)
  : LXCMPlugin ("plugin::Communication", 0x0001, pt)
{
  this->_server = new Server ();
}

LXCMPCommunication::~LXCMPCommunication ()
{
  delete this->_server;
}

void on_message (websocketpp::connection_hdl			hdl,
		 Server::message_ptr				msg)
{
  std::cout << msg->get_payload () << std::endl;
}

void LXCMPCommunication::init ()
{
  this->_pluginTools->log (LXCMLogger::DEBUG, "Initialize Communication Plugin");

  this->_server->set_message_handler (&on_message);

  this->_server->init_asio ();
}

void LXCMPCommunication::start ()
{
  this->_pluginTools->log (LXCMLogger::DEBUG, "Start Communication Plugin");

  this->_server->listen (PORT);
  this->_server->start_accept ();
  this->_server->run ();
}

void LXCMPCommunication::stop ()
{
  this->_pluginTools->log (LXCMLogger::DEBUG, "Stop Communication Plugin");
}

void LXCMPCommunication::quit ()
{
  this->_pluginTools->log (LXCMLogger::DEBUG, "Quit Communication Plugin");
}

extern "C" LXCMPlugin* create (PluginTools* pt)
{
  return new LXCMPCommunication (pt);
}
