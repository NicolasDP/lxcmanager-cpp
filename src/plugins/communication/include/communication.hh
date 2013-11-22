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

#ifndef COMMUNICATION_HH_
# define COMMUNICATION_HH_

# define PORT 9002

# include <deque>
//# include <iostream>

# include <websocketpp/config/asio_no_tls.hpp>
# include <websocketpp/server.hpp>

# include "plugin.hh"
# include "logger.hh"

typedef websocketpp::server<websocketpp::config::asio> Server;

class LXCMPCommunication : public LXCMPlugin
{
  public:
    /*! @brief The plugin constructor
     * Instanciates the websocket server
     */
    LXCMPCommunication ();
    /*! @brief The plugin destructor
     * Deletes the websocket server
     */
    ~LXCMPCommunication ();

  public:
    /*! @brief The mandatory init function
     * Defines the handler when a message is received and initializes
     * the Asio transport system
     */
    void			init ();
    /*! @brief The mandatory start function
     * Defines the listening port, the server default acceptance rule and launch
     * the server.
     */
    void			start ();
    /*! @brief The mandatory stop function
     * Does nothing for now
     */
    void			stop ();
    /*! @brief The mandatory quit function
     * Does nothing for now
     */
    void			quit ();

  private:
    Server*			_server;
    std::string			_moduleName;
//    std::deque<LXCMPConnection> _connections;
};

#endif /* !COMMUNICATION_HH_ */
