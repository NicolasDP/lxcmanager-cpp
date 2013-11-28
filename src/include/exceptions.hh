#ifndef EXCEPTIONS_HH_
# define EXCEPTIONS_HH_

# include <errno.h>
# include <string.h>

# include <exception>
# include <string>
# include <sstream>

class LXCMException: public std::exception
{
  public:

    LXCMException (char const* func,
                   char const* file, int const line,
                   int const code)
      : _function (func)
      , _file (file), _line (line)
      , _code (code), _message (strerror (code))
    {
    } ;

    LXCMException (char const* func,
                   char const* file, int const line,
                   int const code, char const* message)
      : _function (func)
      , _file (file), _line (line)
      , _code (code), _message (message)
    {
    };

    ~LXCMException (void) throw ()
    {
    };

    char const* what (void) const throw ()
    {
      return this->getErrorMessage ().c_str ();
    };

    std::string getErrorMessage (void) const throw ()
    {
      std::ostringstream oss;

      oss << "[" << this->_function << "]"
          << "[" << this->_file << ":" << this->_line << "]"
          << "[" << strerror (this->_code) << "] "
          << this->_message;

      return oss.str ();
    };

    int getCode (void) const throw ()
    {
      return this->_code;
    };
  private:
    std::string _function;
    std::string _file;
    int _line;
    int _code;
    std::string _message;
};

#endif /* !EXCEPTIONS_HH_ */
