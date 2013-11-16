#ifndef OPTIONS_HH_
# define OPTIONS_HH_

# include <boost/program_options.hpp>
# include <iostream>
# include <string>
# include <deque>

namespace po = boost::program_options;

typedef
enum OptionsParseCode
{
	ERR_NONE = 0x0,
	ERR_HELP,
	ERR_ERROR
}
OptionsParseCode;

/* This include should not come before because module.hh needs the definition
 * of the OptionParseCode and the namespace po defined too */
# include "module.hh"

class Options : public LXCMModule
{
	public:
		static Options* getOptions ();
		~Options ();

		void addModule (LXCMModule*);
		void addOption (char const*, char const*);
		void addOption (char const*, po::value_semantic const*, char const*);

		OptionsParseCode parseOptions (int const, char const* const*);
		OptionsParseCode checkOptions (po::variables_map&);

    // int getOptionLevel (std::string);

	private:
		Options ();
		static Options* _singleton;
	private:
		po::options_description* _desc;
		po::variables_map*       _vm;

		std::deque<LXCMModule*>* _modules;
};

#endif /* !OPTIONS_HH_ */
