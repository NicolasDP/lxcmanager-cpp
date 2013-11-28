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

#include "options.hh"
#include "exceptions.hh"
#include "logger.hh"

LXCMOptions* LXCMOptions::_singleton = NULL;

LXCMOptions::LXCMOptions ()
  : LXCMCoreModule ("LXCMOptions")
{
  this->_desc = new po::options_description ("Generic options");
  this->_vm = new po::variables_map ();

  this->_modules = new std::deque<LXCMCoreModule*> ();

  this->_desc->add_options ()
    ("help,h", "produce help message");
}

LXCMOptions::~LXCMOptions ()
{
  delete this->_desc;
  delete this->_vm;
}

LXCMOptions* LXCMOptions::getOptions ()
{
  if (!LXCMOptions::_singleton)
  {
    LXCMOptions::_singleton = new LXCMOptions ();
    LXCMOptions::_singleton->addModule (LXCMOptions::_singleton);
  }

  return LXCMOptions::_singleton;
}

void LXCMOptions::checkOptions (po::variables_map& vm)
{
  if (vm.count ("help"))
  {
    std::cout << *this->_desc << std::endl;
    throw LXCMException (__func__, __FILE__, __LINE__, 0);
  }
}

void LXCMOptions::addModule (LXCMCoreModule* module)
{
  if (module)
  {
    this->_modules->push_back (module);
  }
}

void LXCMOptions::addOption (char const* name, char const* desc)
{
  this->_desc->add_options () (name, desc);
}

void LXCMOptions::addOption (char const* n, po::value_semantic const* v, char const* d)
{
  this->_desc->add_options () (n, v, d);
}

void LXCMOptions::parseOptions (int const argc, char const* const* argv)
{
  std::deque<LXCMCoreModule*>::iterator it;
  std::deque<LXCMCoreModule*>::iterator end;

  try
  {
    po::store (po::parse_command_line (argc, argv, *this->_desc), *this->_vm);
    po::notify (*this->_vm);
  }
  catch (std::exception& e)
  {
    log_message (LXCMLogger::ERROR, e.what ());
    throw LXCMException (__func__, __FILE__, __LINE__, EINVAL, e.what ());
  }

  end = this->_modules->end ();
  for (it = this->_modules->begin (); it != end; it++)
  {
    log_message (LXCMLogger::DEBUG,
                 "core option %s loaded", (*it)->moduleName ().c_str ());
    try
    {
      (*it)->checkOptions (*this->_vm);
    }
    catch (LXCMException& e)
    {
      log_message (LXCMLogger::DEBUG, e.what ());
      throw LXCMException (e.getErrorMessage (),
                           __func__, __FILE__, __LINE__, e.getCode ());
    }
  }
}

