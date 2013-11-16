#ifndef LOGGER_HH_
# define LOGGER_HH_

# include <string>
# include <fstream>

# include "options.hh"

class LXCMLogger : public LXCMModule
{
	public:
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
		static void init ();
		~LXCMLogger ();

		static void log (level const, std::string const&);
		OptionsParseCode checkOptions (po::variables_map&);

	private:
		LXCMLogger ();
		static LXCMLogger* _singleton;

	private:
		level _maxLevel;
		std::ofstream _logOutput;
};

#endif /* !LOGGER_HH_ */
