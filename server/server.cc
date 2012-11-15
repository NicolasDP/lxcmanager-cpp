#include "server.hh"

Server::Server ()
{
}

Server::~Server ()
{
}

void
Server::run ()
{
  listen_sock ();
}

void
Server::listen_sock ()
{
  websocketpp::server::handler::ptr     handler (new Websockethandler ());
  websocketpp::server                  server (handler);

  server.listen (PORT);
}
