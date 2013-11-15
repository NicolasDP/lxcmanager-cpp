#include "options.hh"

Options::Options(int					argc,
		 char**					argv)
{
  po::options_description desc("Allowed options");
  try {
    desc.add_options()
      ("help", "produce help message")
      ("modules-dir", po::value<int>(), "set modules directory")
      ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
  }
  catch (std::exception & e) {
    std::cerr << "error: " << e.what() << std::endl;
  }
}
