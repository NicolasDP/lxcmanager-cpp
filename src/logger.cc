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

#include <exception>
#include <iostream>
#include <ctime>
#include <chrono>

#include <stdio.h>
#include <stdarg.h>

#include "logger.hh"
#include "config.hh"
#include "exceptions.hh"

LXCMLogger* LXCMLogger::_singleton = NULL;

static std::string const level_header[LXCMLogger::NUMBER_OF_LOG_LEVEL] =
{
  [LXCMLogger::ERROR] = "ERROR",
  [LXCMLogger::INFO]  = "INFO",
  [LXCMLogger::DEBUG] = "DEBUG"
};

LXCMLogger::LXCMLogger ()
  : LXCMCoreModule ("LXCMLogger")
  , _logOutput ()
{
  LXCMOptions* opt = LXCMOptions::getOptions ();

  this->_maxLevel = (LXCMLogger::level)CONFIG_CORE_DEFAULT_LOG_LEVEL;

  opt->addModule (this);
  opt->addOption ("log-level",
                  po::value<int> (),
                  "set log level mask:\n"
                  "    0: Errors Only\n"
                  "    1: Infos and Errors only\n"
                  "    2: Debug, Infos and Errors");
  opt->addOption ("log-file",
                  po::value<std::string> (),
                  "set log file to write all of the log messages. If "
                  "not set, log messages are redirected to the error "
                  "output");
}

LXCMLogger::~LXCMLogger ()
{
  this->_logOutput.close ();
}

void LXCMLogger::init ()
{
  if (!LXCMLogger::_singleton)
  {
    LXCMLogger::_singleton = new LXCMLogger ();
  }
}

void LXCMLogger::checkOptions (po::variables_map& vm)
{
  if (vm.count ("log-level"))
  {
    int lvl;
    /* get the default value and do some update */
    lvl = vm["log-level"].as<int> ();
    switch (lvl)
    {
      /* It's more logic to think like this:
       * If we set DEBUG logs, we also enable INFO and ERROR logs
       * But if we set ERROR logs, we jump directly to the ERROR case and
       * don't enable the "higher logs" */
      case DEBUG:
      case INFO:
      case ERROR:
        this->_maxLevel = (LXCMLogger::level) lvl;
        break;
      case NUMBER_OF_LOG_LEVEL:
      default:
        log_message (LXCMLogger::ERROR, "Log level (%d) not available", lvl);
        THROW_ERROR (EINVAL, "Log level not available");
    }
  }

  if (vm.count ("log-file"))
  {
    std::string filename;
    filename = vm["log-file"].as<std::string> ();
    /* Open the file */
    try
    {
      this->_logOutput.open (filename.c_str (),
                             std::ofstream::out | std::ofstream::app);
    }
    catch (std::exception& e)
    {
      log_message (LXCMLogger::ERROR, "%s", e.what ());
      THROW_ERROR (EINVAL, e.what ());
    }
  }
}

void LXCMLogger::log (LXCMLogger::level const lvl, std::string const& message)
{
  if (lvl <= LXCMLogger::_singleton->_maxLevel)
  {
    if (LXCMLogger::_singleton->_logOutput.is_open ())
    {
      std::chrono::system_clock::time_point now = std::chrono::system_clock::now ();
      std::time_t tt = std::chrono::system_clock::to_time_t (now);
      LXCMLogger::_singleton->_logOutput
        << "[" << tt << "]"
        << "[" << CONFIG_PROJECT_NAME << "]"
        << "[" << level_header[lvl] << "] "
        << message << std::endl;
    }
    else
    {
      std::clog << "[" << level_header[lvl] << "] "
                << message << std::endl;
    }
  }
}

void __attribute__ ((constructor)) coremodule_init_logger (void)
{
  LXCMLogger::init ();
}

void log_message (LXCMLogger::level lvl, char const* fmt, ...)
{
  std::string message;
  char tmp[256];
  va_list ap;

  va_start (ap, fmt);
  vsnprintf (tmp, sizeof (tmp), fmt, ap);
  va_end (ap);

  message = tmp;
  LXCMLogger::log (lvl, message);
}
