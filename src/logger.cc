#include <exception>
#include <iostream>
#include <ctime>
#include <chrono>

#include "logger.hh"
#include "config.hh"

LXCMLogger* LXCMLogger::_singleton = NULL;

static std::string const level_header[LXCMLogger::NUMBER_OF_LOG_LEVEL] =
{
	[LXCMLogger::ERROR] = "ERROR",
	[LXCMLogger::INFO]  = "INFO",
	[LXCMLogger::DEBUG] = "DEBUG"
};

LXCMLogger::LXCMLogger () : _logOutput ()
{
	Options* opt = Options::getOptions ();

	this->_maxLevel = LXCMLogger::ERROR;

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

	this->_moduleName = "LXCMLXCMLogger";
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

OptionsParseCode LXCMLogger::checkOptions (po::variables_map& vm)
{
	if (vm.count ("log-level"))
	{
		int lvl;
		/* get the default value and do some update */
		lvl = vm["log-level"].as<int> ();
		switch (lvl)
		{
		case ERROR:
		case INFO:
		case DEBUG:
			this->_maxLevel = (LXCMLogger::level) lvl;
			break;
		case NUMBER_OF_LOG_LEVEL:
		default:
			return ERR_ERROR;
		}
	}

	if (vm.count ("log-file"))
	{
		std::string filename;
		filename = vm["log-file"].as<std::string> ();
		std::cerr << "open (" << filename << ")" << std::endl;
		/* Open the file */
		/* TODO: manage error (throw something...) */
		try
		{
			this->_logOutput.open (filename.c_str (),
			               std::ofstream::out | std::ofstream::app);
			if (LXCMLogger::_singleton->_logOutput.is_open ())
				std::cerr << "file " << filename << " is open" << std::endl;

		}
		catch (std::exception& e)
		{
			std::cerr << e.what () << std::endl;
			this->log (LXCMLogger::ERROR,
			           e.what ());
		}
	}

	return ERR_NONE;
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
			    << "[" << PROJECT_NAME << "]"
			    << "[" << level_header[lvl] << "] "
			    << message << std::endl;
		}
		else
		{
			std::clog
			    << "[" << level_header[lvl] << "] "
			    << message << std::endl;
		}
	}
}