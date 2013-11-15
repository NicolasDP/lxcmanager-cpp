#ifndef EXCEPTIONS_HH_
# define EXCEPTIONS_HH_

#include <iostream>
#include <exception>

using namespace std;

class myexception: public exception
{
    virtual const char* what() const throw()
    {
      return "Exit properly all modules";
    }
};

#endif /* !EXCEPTIONS_HH_ */
