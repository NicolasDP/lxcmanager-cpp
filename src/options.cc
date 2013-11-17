#include "options.hh"
#include "logger.hh"

LXCMOptions* LXCMOptions::_singleton = NULL;

LXCMOptions::LXCMOptions ()
{
  this->_desc = new po::options_description ("Generic options");
  this->_vm = new po::variables_map ();

  this->_modules = new std::deque<LXCMCoreModule*> ();

  this->_desc->add_options ()
    ("help,h", "produce help message");

  this->_moduleName = "LXCMLXCMOptions";
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

OptionsParseCode LXCMOptions::checkOptions (po::variables_map& vm)
{
  if (vm.count ("help"))
  {
    std::cout << *this->_desc << std::endl;
    return ERR_HELP;
  }

  return ERR_NONE;
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

OptionsParseCode LXCMOptions::parseOptions (int const argc, char const* const* argv)
{
  std::deque<LXCMCoreModule*>::iterator it;
  std::deque<LXCMCoreModule*>::iterator end;
  OptionsParseCode ret = ERR_NONE;

  try
  {
    po::store (po::parse_command_line (argc, argv, *this->_desc), *this->_vm);
    po::notify (*this->_vm);
  }
  catch (std::exception& e)
  {
    std::cerr << e.what () << std::endl;
    return ERR_ERROR;
  }

  end = this->_modules->end ();
  for (it = this->_modules->begin (); it != end; it++)
  {
    LXCMLogger::log (LXCMLogger::INFO, (*it)->moduleName ());
    ret = (*it)->checkOptions (*this->_vm);
    if (ERR_NONE != ret)
    {
      break;
    }
  }

  return ret;
}

