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

#include <iostream>

#include "communication.hh"

/* To handle LXCM exceptions (essentially thowing) */
#include "exceptions.hh"
/* To get the level enumeration symbol */
#include "logger.hh"
/* To link and use the JsonVal in order to manage specific configuration
 * options */
#include "lxcmjson.hh"

LXCMPCommunication::LXCMPCommunication (PluginTools* pt)
  : LXCMPlugin ("plugin::Communication", 0x0001, pt)
  , _port (COMMUNICATION_DEFAULT_PORT)
{
  this->_server = new WSServer ();
}

LXCMPCommunication::~LXCMPCommunication ()
{
  delete this->_server;
}

void on_message (websocketpp::connection_hdl hdl, WSServer::message_ptr msg)
{
  std::cout << msg->get_payload () << std::endl;
}

void LXCMPCommunication::init ()
{
  this->_pluginTools->log (LXCMLogger::DEBUG, "Initialize Communication Plugin");

  this->_server->set_message_handler (&on_message);
  this->_server->init_asio ();

  /* Get the configuration options specified by the configuration file: */
  LXCMJsonVal const* config = this->_pluginTools->get_my_options (this);
  if (config && config->_type == JSON_OBJECT_BEGIN)
  {
    LXCMJsonVal const* port = config->getObject ("port");
    if (port && port->_type == JSON_INT)
    {
      std::istringstream (port->_data) >> this->_port;
    }
  }
}

void LXCMPCommunication::start ()
{
  this->_pluginTools->log (LXCMLogger::DEBUG, "Start Communication Plugin");

  this->_server->listen (this->_port);
  this->_server->start_accept ();
  this->_server->run ();
}

void LXCMPCommunication::stop ()
{
  this->_pluginTools->log (LXCMLogger::DEBUG, "Stop Communication Plugin");
}

void LXCMPCommunication::receive (LXCMPlugin* from, std::string& message)
{
  if (this->_lockMessage)
  {
    THROW_ERROR (EBUSY, "message already locked");
  }

  if (from)
  {
    this->_lockMessage = true;
    this->_pluginTools->log (LXCMLogger::DEBUG, "receive message '%s' from %s",
                             message.c_str (), from->moduleName ().c_str ());
    this->_lockMessage = false;
  }
}

void LXCMPCommunication::quit ()
{
  this->_pluginTools->log (LXCMLogger::DEBUG, "Quit Communication Plugin");
}

extern "C" LXCMPlugin* create (PluginTools* pt)
{
  return new LXCMPCommunication (pt);
}
