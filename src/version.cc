#include "version.hh"

LXCMVersion* LXCMVersion::_singleton = NULL;

LXCMVersion::LXCMVersion ()
{
	Options* opts = Options::getOptions ();

	opts->addModule (this);
	opts->addOption ("version", "print version");
}

LXCMVersion::~LXCMVersion ()
{
	LXCMVersion::_singleton = NULL;
}

void LXCMVersion::init ()
{
	if (!LXCMVersion::_singleton)
	{
		LXCMVersion::_singleton = new LXCMVersion ();
	}
}

OptionsParseCode LXCMVersion::checkOptions (po::variables_map& vm)
{
	if (vm.count ("version"))
	{
		std::cout << PROJECT_NAME << " version "
		          << PROJECT_VERSION_STRING
		          <<  std::endl;
	}

	return ERR_HELP;
}
