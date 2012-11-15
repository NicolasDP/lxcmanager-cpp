#include "main.hh"

int             main (int               argc,
                      char**            argv)
{
  Server*       srv = new Server ();

  srv->run ();
  delete srv;
}
