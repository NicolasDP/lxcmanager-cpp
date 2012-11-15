#ifndef WEBSOCKETHANDLER_HH_
# define WEBSOCKETHANDLER_HH_

# include <iostream>

# include "../websocketpp-api/roles/client.hpp"
# include "../websocketpp-api/websocketpp.hpp"

class Websockethandler : public websocketpp::client::handler
{
  public:
    void        on_fail (connection_ptr         con);
    void        on_open (connection_ptr         con);
    void        on_close (connection_ptr        con);
    void        on_message (connection_ptr      con,
                            message_ptr         msg);
    void        send (const std::string         msg);
    void        close ();
  private:
    websocketpp::client::connection_ptr         con_;
};

typedef boost::shared_ptr<Websockethandler> client_handler_ptr;

#endif /* !WEBSOCKETHANDLER_HH_ */
