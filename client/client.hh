#ifndef CLIENT_HH_
# define CLIENT_HH_

# include <iostream>
# include <boost/asio.hpp>

# include "websockethandler.hh"

# define INVALID_SOCKET -1
# define SOCKET_ERROR -1
# define WSURI "ws://localhost:6000/"

/**
 * \brief Class representing the client daemon
 *
 * This class is the main one in the programm.
 *
 */
class Client
{
  public:
    Client ();
    ~Client ();
  /**
   * \brief Start connection
   *
   * Start the loop which connect to the server
   *
   */
    void                run ();
  /**
   * \brief Connect to the server
   *
   * Bind client socket to server
   *
   */
    void                connect_sock ();
  private:
    websocketpp::client::connection_ptr         con_;
    websocketpp::client                         client_;
    client_handler_ptr                          handler_;
    std::string                                 wsuri_;
};

#endif /* !CLIENT_HH_ */
