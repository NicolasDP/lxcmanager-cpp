#include "websockethandler.hh"

void
Websockethandler::on_fail (connection_ptr       con)
{
  std::cout << "Connection failed" << std::endl;
}

void
Websockethandler::on_open (connection_ptr       con)
{
  con_ = con;
  std::cout << "Successfully connected" << std::endl;
}

void
Websockethandler::on_close (connection_ptr      con)
{
  con_ = websocketpp::client::connection_ptr ();
  std::cout << "client connection is closed" << std::endl;
}

void
Websockethandler::on_message (connection_ptr    con,
                              message_ptr       msg)
{
  std::cout << "client receive message" << std::endl;
  std::cout << "message : " << msg->get_payload () << std::endl;
}

void
Websockethandler::send (const std::string       msg)
{
  if (!con_)
  {
    std::cerr << "Error: no connected session" << std::endl;
    return;
  }

  if (msg == "/close")
    close ();
  else
    con_->send (msg);
}

void
Websockethandler::close ()
{
  if (!con_)
  {
    std::cerr << "Error: no connected session" << std::endl;
    return;
  }
  con_->close(websocketpp::close::status::GOING_AWAY,"");
}
