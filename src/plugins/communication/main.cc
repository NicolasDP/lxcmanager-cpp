#include "communication.hh"

int main()
{
  LXCMPCommunication com;

  com.init ();
  com.start ();

  return 0;
}
