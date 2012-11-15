/**
 * \file main.hh
 * \brief Server main class
 * \author Zaccaria Dorian
 */

#ifndef SERVER_HH_
# define SERVER_HH_

# include <iostream>
# include <boost/asio.hpp>

# include "websockethandler.hh"

# define PORT 6000


/**
 * \brief Class representing the server daemon
 *
 * This class is the main one in the programm.
 *
 */
class Server
{
  public:
    Server ();
    ~Server ();
  /**
   * \brief Start server
   *
   * Start the websocket server which listen client connections
   *
   */
    void                run ();
  /**
   * \brief Listen on the socket
   *
   * Listen on the socket
   *
   */
    void                listen_sock ();
};

#endif /* !SERVER_HH_ */
