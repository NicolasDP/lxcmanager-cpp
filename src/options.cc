#include "options.hh"

Options::Options (int const argc, char const * const * argv)
{
	this->_desc = new po::options_description ("Allowed options");
	try
	{
		this->_desc->add_options ()
			("help", "produce help message")
			("modules-dir", po::value<int> (), "set modules directory");
		this->_vm = new po::variables_map ();
		po::store (po::parse_command_line (argc, argv, *this->_desc), *this->_vm);
	}
	catch (std::exception & e)
	{
		std::cerr << "error: " << e.what () << std::endl;
	}
}

Options::~Options ()
{
	if (this->_desc)
	{
		delete this->_desc;
	}

	if (this->_vm)
	{
		delete this->_vm;
	}
}
