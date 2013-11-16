#include "options.hh"
#include "logger.hh"

Options* Options::_singleton = NULL;

Options::Options ()
{
	this->_desc = new po::options_description ("Generic options");
	this->_vm = new po::variables_map ();

	this->_modules = new std::deque<LXCMModule*> ();

	this->_desc->add_options ()
	    ("help,h", "produce help message");

	this->_moduleName = "LXCMOptions";
}

Options::~Options ()
{
	delete this->_desc;
	delete this->_vm;
}

Options* Options::getOptions ()
{
	if (!Options::_singleton)
	{
		Options::_singleton = new Options ();
		Options::_singleton->addModule (Options::_singleton);
	}

	return Options::_singleton;
}

OptionsParseCode Options::checkOptions (po::variables_map& vm)
{
	if (vm.count ("help"))
	{
		std::cout << *this->_desc << std::endl;
		return ERR_HELP;
	}

	return ERR_NONE;
}

void Options::addModule (LXCMModule* module)
{
	if (module)
	{
		this->_modules->push_back (module);
	}
}

void Options::addOption (char const* name, char const* desc)
{
	this->_desc->add_options () (name, desc);
}

void Options::addOption (char const* n, po::value_semantic const* v, char const* d)
{
	this->_desc->add_options () (n, v, d);
}

OptionsParseCode Options::parseOptions (int const argc, char const* const* argv)
{
	std::deque<LXCMModule*>::iterator it;
	std::deque<LXCMModule*>::iterator end;
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

