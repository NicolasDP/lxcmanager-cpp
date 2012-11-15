#include "client.hh"

Client::Client ()
  :handler_ (new Websockethandler ())
  ,client_ (handler_)
  ,wsuri_ ("ws://localhost:6000/")
{
}

Client::~Client ()
{
}

void
Client::run ()
{
  connect_sock ();
}

void
Client::connect_sock ()
{
  con_ = client_.get_connection (wsuri_);

  con_->add_request_header("User-Agent","WebSocket++/0.2.0 WebSocket++LxcManager/0.2.0");
  con_->add_subprotocol("com.zaphoyd.websocketpp.LxcManager");

  con_->set_origin("http://zaphoyd.com");

  client_.connect (con_);

  boost::thread t(boost::bind(&websocketpp::client::run, &client_, false));

  char line[512];
  while (std::cin.getline(line, 512)) {
    handler_->send(line);
  }

  t.join();
  // con_->add_request_header("User-Agent",
  //                          "WebSocket++/0.2.0 WebSocket++Chat/0.2.0");
  // con_->add_subprotocol("com.zaphoyd.websocketpp.chat");
  // con_->set_origin("http://zaphoyd.com");
  // client_->connect (con_);
  // boost::bind (&websocketpp::client::run, client_, true);
}
