/* This file is part of LXCManager.
 *
 * LXCManager is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LXCManager is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LXCManager.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef MODULE_HH_
# define MODULE_HH_

# include "options.hh"

/*
 * @class LXCMModule
 *
 * Most of the part of this project should inherit from this class.
 * By implemented this methods you will be able to handle specific option for
 * your module.
 *
 * A minimal son may be:
 * class Foo : public LXCMModule
 * {
 *      public:
 *          Foo ()
 *          {
 *              LXCMOptions* opts = LXCMOption::getOptions ()
 *              opts->addModule (this);
 *              opts->addOption ("foo,f", "useless option");
 *              opts->addOption ("bar", po::value<int>, "useless too");
 *
 *              this->_moduleName = "My name is Foo";
 *          };
 *
 *          ~Foo () {};
 *          OptionParseCode checkOptions (po::variables_map& vm)
 *          {
 *               if (vm.count ("foo")) {
 *                   std::cout << "FOO" << std::endl;
 *               }
 *
 *               if (vm.count ("bar"))
 *               {
 *                   std::cout << "BAR " << vm["bar"].as<int> () << std::endl;
 *               }
 *          };
 * };
 */
class LXCMModule
{
	public:
		/**
		 * @brief virtual interface that son must implement
		 * It is use to handle the options it sets
		 * If there is no options to handle, then this function does
		 * nothing but must be implemented.
		 *
		 * @param po::variables_map&: contains the list of options
		 * given by the user (see libboost::program_options). */
		virtual OptionsParseCode checkOptions (po::variables_map&) = 0;
		/** @brief The moduleName getter, it returns the name of the
		 * current module. Useful for debug and information logs.
		 *
		 * @return it returns the name of the module (_moduleName) */
		virtual std::string& moduleName ()
		{
			return this->_moduleName;
		}

	protected:
		/** Module name (all module must set this field - use for debug
		 *  and information log). */
		std::string _moduleName;
};

#endif /* !MODULE_HH_ */
