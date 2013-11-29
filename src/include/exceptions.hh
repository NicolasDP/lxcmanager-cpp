/* This file is part of LXCManager.
 *
 * LXCManager is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LXCManager is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LXCManager.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef EXCEPTIONS_HH_
# define EXCEPTIONS_HH_

# include <errno.h>
# include <string.h>

# include <exception>
# include <string>
# include <sstream>

# include "config.hh"

# define THROW_ERROR_CODE(code)                               \
  do {                                                        \
    throw LXCMException (__func__, __FILE__, __LINE__, code); \
  } while (0)

# define THROW_ERROR(code, message)                                    \
  do {                                                                 \
    throw LXCMException (__func__, __FILE__, __LINE__, code, message); \
  } while (0)

# if defined (CONFIG_DEBUG) && CONFIG_DEBUG
#  define THROW_FORWARD_ERROR(e)                         \
    do {                                                 \
      throw LXCMException (e.getErrorMessage (),         \
                           __func__, __FILE__, __LINE__, \
                           e.getCode ());                \
    } while (0)
# else
#  define THROW_FORWARD_ERROR(e)                         \
   THROW_ERROR (e.getCode (), e.getMessage ())
#endif

/*! @class LXCMException
 *
 * This is the common exception Class that lxcmanager's plugins or cores's
 * methodes should support. It allows us to follow the call trace and report
 * easily and in the most understable way error reporting and handleling.
 *
 * See the constructors' comments for more information.
 *
 * @code
 * void func1 (void)
 * {
 *   trow LXCMException (__func__, __FILE__, __LINE__,
 *                       EINVAL, "bad args...");
 * }
 *
 * void func2 (void)
 * {
 *   try {
 *     func1 ();
 *   } catch (LXCMException& e) {
 *     throw LXCMException (e.getErrorMessage (),
 *                          __func__, __FILE__, __LINE__,
 *                          e.getCode ());
 *   }
 * }
 *
 * void main (void)
 * {
 *   try {
 *     func2 ();
 *   } catch (LXCMException& e) {
 *     std::clog << e.getErrorMessage () << std::endl;
 *   }
 * }
 * @endcode
 *
 * The output may be:
 *
 * @code
 *  $ [func1][main.cc:3][Invalid argument] bad args...
 *  $ [func2][main.cc:10][Invalid argument] Invalid argument
 * @endcode */
class LXCMException: public std::exception
{
  public:
    /*! @brief Constructor without exception forwarding.
     *
     * This constructor will build the LXCMException without any previous
     * message (i.e. without previous exception message).
     *
     * @param func function name (use the gcc macro \_\_func\_\_)
     * @param file file name (use \_\_FILE\_\_)
     * @param line line number (use \_\_LINE\_\_)
     * @param code is the error code (use errno code, see man 2 errno) */
    LXCMException (char const* func,
                   char const* file, int const line,
                   int const code)
      : _have_upper (false), _upper (""), _function (func)
      , _file (file), _line (line)
      , _code (code), _message (strerror (code))
    {
    } ;

    /*! @brief Constructor without exception forwarding.
     *
     * This constructor will build the LXCMException without any previous
     * message (i.e. without previous exception message).
     *
     * @param func function name (use the gcc macro \_\_func\_\_)
     * @param file file name (use \_\_FILE\_\_)
     * @param line line number (use \_\_LINE\_\_)
     * @param code is the error code (use errno code, see man 2 errno)
     * @param message a message string */
    LXCMException (char const* func,
                   char const* file, int const line,
                   int const code, char const* message)
      : _have_upper (false), _upper (""), _function (func)
      , _file (file), _line (line)
      , _code (code), _message (message)
    {
    };

    /*! @brief Constructor with exception forwarding.
     *
     * This constructor will build the LXCMException with a previous
     * message (i.e. with previous exception message).
     *
     * @param upper the result of the previous exception
     * @param func function name (use the gcc macro \_\_func\_\_)
     * @param file file name (use \_\_FILE\_\_)
     * @param line line number (use \_\_LINE\_\_)
     * @param code is the error code (use errno code, see man 2 errno) */
    LXCMException (std::string upper, char const* func,
                   char const* file, int const line,
                   int const code)
      : _have_upper (true), _upper (upper), _function (func)
      , _file (file), _line (line)
      , _code (code), _message ("")
    {
    } ;

    /*! @brief Constructor with exception forwarding.
     *
     * This constructor will build the LXCMException with a previous
     * message (i.e. with previous exception message).
     *
     * @param upper the result of the previous exception
     * @param func function name (use the gcc macro \_\_func\_\_)
     * @param file file name (use \_\_FILE\_\_)
     * @param line line number (use \_\_LINE\_\_)
     * @param code is the error code (use errno code, see man 2 errno)
     * @param message a message string */
    LXCMException (std::string upper, char const* func,
                   char const* file, int const line,
                   int const code, char const* message)
      : _have_upper (true), _upper (upper), _function (func)
      , _file (file), _line (line)
      , _code (code), _message (message)
    {
    };

    /*! @brief destructor */
    ~LXCMException (void) throw ()
    {
    };

    /*! @brief redifined the std::exception's method */
    char const* what (void) const throw ()
    {
      return this->getErrorMessage ().c_str ();
    };

    /*! @brief return the message string */
    std::string getErrorMessage (void) const throw ()
    {
      std::ostringstream oss;

      if (this->_have_upper)
      {
        oss << this->_upper << std::endl << "[FORWARD] ";
      }

#if defined (CONFIG_DEBUG) && CONFIG_DEBUG
      oss << "[" << this->_function << "]"
          << "[" << this->_file << ":" << this->_line << "]"
          << "[" << strerror (this->_code) << "] "
          << this->_message;
#else
      oss << "[" << strerror (this->_code) << "] "
          << this->_message;
#endif

      return oss.str ();
    };

    /*! @brief return the error code */
    int getCode (void) const throw ()
    {
      return this->_code;
    };

    /*! @brief return the message */
    char const* getMessage (void) const throw ()
    {
      return this->_message.c_str ();
    };
  private:
    bool _have_upper;
    std::string _upper;
    std::string _function;
    std::string _file;
    int _line;
    int _code;
    std::string _message;
};

#endif /* !EXCEPTIONS_HH_ */
