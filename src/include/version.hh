#ifndef LXCMVERSION_HH_
# define LXCMVERSION_HH_

# include "options.hh"
# include "config.hh"

class LXCMVersion : public LXCMCoreModule
{
	public:
		static void init ();
		~LXCMVersion ();

		OptionsParseCode checkOptions (po::variables_map&);

	private:
		LXCMVersion ();
		static LXCMVersion* _singleton;
};

#endif /* !LXCMVERSION_HH_ */
