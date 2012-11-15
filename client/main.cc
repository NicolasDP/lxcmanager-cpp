#include "main.hh"

int             main (int               argc,
                      char**            argv)
{
  Client*       clt = new Client ();

  clt->run ();
  delete clt;
}
