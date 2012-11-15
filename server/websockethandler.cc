#include "websockethandler.hh"

void
Websockethandler::validate (connection_ptr      con)
{
  std::cout << con->get_resource () << " is valide" << std::endl;
  if (con->get_resource () != "/LxcManager")
    std::cout << "unauthentificate user" << std::endl;
}

void
Websockethandler::on_fail (connection_ptr       con)
{
  std::cout << con->get_resource () << " is failed" << std::endl;
}

void
Websockethandler::on_open (connection_ptr       con)
{
  std::cout << con->get_resource () << " is open" << std::endl;
  std::cout << "client " << con << " is connected" << std::endl;

  con->send ("coucou");
}

void
Websockethandler::on_close (connection_ptr      con)
{
  std::cout << con->get_resource () << " is closed" << std::endl;
}

void
Websockethandler::on_message (connection_ptr    con,
                              message_ptr       msg)
{
  std::cout << con->get_resource () << " is message ?" << std::endl;
  std::cout << "message : " << msg->get_payload () << std::endl;
}
