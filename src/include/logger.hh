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

#ifndef LOGGER_HH_
# define LOGGER_HH_

# include <string>
# include <fstream>

# include "options.hh"

/*!
 * @class LXCMLogger
 *
 * Logger interfaces, it is a tool to log messages (ERROR, INFO, DEBUG...).
 * Example:
 *   LXCMLogger::log (LXCMLogger::ERROR, "this is an error message");
 *
 * More information about how to setup the message log mask or the log message
 * destination (a file instead of the std::log output) see the constructor. */
class LXCMLogger : public LXCMCoreModule
{
  public:
    /*! @enum level
     * It defines the log message levels */
    typedef
    enum
      {
        ERROR = 0,
        INFO,
        DEBUG,

        NUMBER_OF_LOG_LEVEL /* NEVER USE IT or upper number */
      }
    level;

  public:
    /*! @brief init initialize the singleton instance */
    static void init ();
    /*! @brief destructor */
    ~LXCMLogger ();

    /*! @brief log
     * This function gives the main interfaces to throw a log message. It will
     * be printed in the standard log output (std::clog) or in a file given by
     * the option command line (--log-file path/to/log).
     *
     * @param level the level of this debug message
     * @param std::string the log message */
    static void log (level const, std::string const&);
    /*! @brief checkOptions
     * it implements the virtual method defined in the
     * LXCMCoreModule. */
    OptionsParseCode checkOptions (po::variables_map&);

  private:
    /*! @brief constructor */
    LXCMLogger ();
    /*! @brief the unique instance of this Class */
    static LXCMLogger* _singleton;

  private:
    /*! @brief the log message mask level, modified by the command line option
     * --log-level. */
    level _maxLevel;
    /*! @brief the log file, setted by the command line --log-file. If this
     * option is not given then the log message will be printed in the standard
     * log output (std::clog). */
    std::ofstream _logOutput;
};

void log_message (LXCMLogger::level lvl, char const* fmt, ...);

#endif /* !LOGGER_HH_ */
