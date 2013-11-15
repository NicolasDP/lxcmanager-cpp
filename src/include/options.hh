#ifndef OPTIONS_HH_
# define OPTIONS_HH_

#include <boost/program_options.hpp>
#include <iostream>
#include <string>

namespace po = boost::program_options;

class Options
{
	public:
		Options (int const argc, char const * const * argv);
		~Options ();

	private:
		po::options_description * _desc;
		po::variables_map *       _vm;
};

#endif /* !OPTIONS_HH_ */
