#include "logger.hh"

Logger* Logger::_singleton = NULL;

Logger::Logger ()
{
}

Logger::~Logger ()
{
}

Logger* Logger::getLogger ()
{
  if (!Logger::_singleton)
  {
    Logger::_singleton = new Logger ();
    Options* opt = Options::getOptions ();
    opt->addModule (Logger::_singleton);

    // _maxLevel = opt->getOptionLevel ("log-level");
  }

  return Logger::_singleton;
}

OptionsParseCode Logger::checkOptions (po::variables_map& vm)
{
	if (vm.count ("help"))
	{
		return ERR_HELP;
	}

	return ERR_NONE;
}

void Logger::Log (std::string message, int type, int level)
{
  switch (type)
  {
    case 0:
      this->LogStandard (message, level);
      break;
    case 1:
      this->LogError (message, level);
      break;
    default:
      break;
  }
}

void Logger::LogStandard (std::string msg, int lvl)
{
  if (lvl <= _maxLevel)
    std::cout << "[+] [" << lvl << "]" << std::setw (5) << msg << std::endl;
}

void Logger::LogError (std::string msg, int lvl)
{
  if (lvl <= _maxLevel)
    std::cerr << "[-] [" << lvl << "]" << std::setw (5) << msg << std::endl;
}
