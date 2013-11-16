#ifndef EXCEPTIONS_HH_
# define EXCEPTIONS_HH_

# include <iostream>
# include <exception>
# include <string>

using namespace std;

class LXCMException: public exception
{
//  public:
    // LXCMException (std::string, std::string, int);
    // ~LXCMException ();

    virtual const char* what () const throw ()
    {
      return "Exit";
    }

  // private:
  //   std::string _funcName;
  //   std::string _fileName;
  //   int _line;
};

#endif /* !EXCEPTIONS_HH_ */
