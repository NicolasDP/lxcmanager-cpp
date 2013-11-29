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

#ifndef LXCMOPTIONS_HH_
# define LXCMOPTIONS_HH_

# include <boost/program_options.hpp>
# include <iostream>
# include <string>
# include <deque>

/*!
 * @namespace po
 * a short cut for boost::program_options */
namespace po = boost::program_options;

/* This include should not come before because module.hh needs the definition
 * of the OptionParseCode and the namespace po defined too */
# include "coremodule.hh"

/*!
 * @class LXCMOptions
 * It's implement an options module */
class LXCMOptions : public LXCMCoreModule
{
  public:
    /*! @brief getOptions
     *  returns the LXCMOptions instance and initializes it if
     *  needed.
     *  @return LXCMOptions, the unique instance of this Class */
    static LXCMOptions* getOptions ();
    /*! @brief destructor */
    ~LXCMOptions ();

    /*! @brief addModule
     *  it saves the instance LXCModule address in the _modules
     *  deque. At the parsing time, the parseOptions methode will
     *  be call for each registered LXCModule.
     *
     *  @param module is the address of a LXCModule instance */
    void addModule (LXCMCoreModule* module);
    /*! @brief addOption
     *  it adds an option in the boost::program_options
     *  description. This option doesn't expect an argement.
     *
     *  @param opt is the option string "long,s" where long
     *  is a long option (--long) and s is the associated shord
     *  option (-s).
     *  @param desc is the option description */
    void addOption (char const* opt, char const* desc);
    /*! @brief addOption
     *  it does the same but with an argument required.
     *
     *  @param opt is the option string "long,s" where long
     *  is a long option (--long) and s is the associated shord
     *  option (-s).
     *  @param arg defines the type of a
     *  required argument. See boost::program_option for more
     *  information.
     *  @param desc is the option's description */
    void addOption (char const* opt, po::value_semantic const* arg,
                    char const* desc);

    /*! @brief parseOptions
     *  it parses all of the registered options from the registered
     *  module (see addOption and addModule functions).
     *
     *  @param argc is the number of argument given in command
     *  line option.
     *  @param argv is the argument array givent in
     *  command line option. */
    void parseOptions (int const argc, char const* const* argv);
    /*! @brief checkOptions (see LXCModule) */
    void checkOptions (po::variables_map&);

  private:
    /*! @brief constructor */
    LXCMOptions ();
    /*! @brief singleton field (unique value) see Singleton design
     * pattern */
    static LXCMOptions* _singleton;
  private:
    po::options_description _desc;/*!< see bosst::program_option */
    po::variables_map _vm;/*!< see bosst::program_option */

    std::deque<LXCMCoreModule*> _modules; /*!< instance of each registered LXCModule */
};

#endif /* !LXCMOPTIONS_HH_ */
