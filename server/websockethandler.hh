/**
 * \file websockethandler.hh
 * \brief The main class which permit to communicate with websocket
 * \author Zaccaria Dorian
 */

#ifndef WEBSOCKETHANDLER_HH_
# define WEBSOCKETHANDLER_HH_

# include "../websocketpp-api/websocketpp.hpp"

class Websockethandler : public websocketpp::server::handler
{
  public:
    void        validate (connection_ptr        con);
    void        on_fail (connection_ptr         con);
    void        on_open (connection_ptr         con);
    void        on_close (connection_ptr        con);
    void        on_message (connection_ptr      con,
                            message_ptr         msg);
};

#endif /* !WEBSOCKETHANDLER_HH_ */
