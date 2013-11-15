#ifndef OPTIONS_HH_
# define OPTIONS_HH_

#include <boost/program_options.hpp>
#include <iostream>
#include <string>

namespace po = boost::program_options;

class Options
{
  public:
    Options(int						argc,
	    char**					argv);
    ~Options();

  private:
    po::options_description				desc_;
    po::variables_map					vm_;
};

#endif /* !OPTIONS_HH_ */
