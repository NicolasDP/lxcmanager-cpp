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

/*!
 * @enum OptionsParseCode
 * it's the return code used to chose to leave the program (with an error or
 * not) or to continue the main execution */
typedef
enum OptionsParseCode
{
  ERR_NONE = 0x0, /*!< no error, continue */
  ERR_HELP,       /*!< no error, but user requested for something else
                   * which needs to leave without executing the program */
  ERR_ERROR       /*!< an error occured, leave the program as soon as
                   * possible */
}
OptionsParseCode;

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
     *  @param LXCModule*: the address of a LXCModule instance */
    void addModule (LXCMCoreModule*);
    /*! @brief addOption
     *  it adds an option in the boost::program_options
     *  description. This option doesn't expect an argement.
     *
     *  @param char const*: the option string "long,s" where long
     *  is a long option (--long) and s is the associated shord
     *  option (-s).
     *  @param char const*: the option description */
    void addOption (char const*, char const*);
    /*! @brief addOption
     *  it does the same but with an argument required.
     *
     *  @param char const*: the option string "long,s" where long
     *  is a long option (--long) and s is the associated shord
     *  option (-s).
     *  @param po::value_semantic which defines the type of a
     *  required argement. See boost::program_option for more
     *  information.
     *  @param char const*: the option description */
    void addOption (char const*, po::value_semantic const*, char const*);

    /*! @brief parseOptions
     *  it parses all of the registered options from the registered
     *  module (see addOption and addModule functions).
     *
     *  @param int const: the number of argument given in command
     *  line option (argc).
     *  @param char const* const* the argument array givent in
     *  command line option (argv).
     *
     *  @return OptionsParseCode
     *  see OptionsParseCode */
    OptionsParseCode parseOptions (int const, char const* const*);
    /*! @brief checkOptions (see LXCModule) */
    OptionsParseCode checkOptions (po::variables_map&);

  private:
    /*! @brief constructor */
    LXCMOptions ();
    /*! @brief singleton field (unique value) see Singleton design
     * pattern */
    static LXCMOptions* _singleton;
  private:
    /*! @brief see bosst::program_option */
    po::options_description* _desc;
    /*! @brief see bosst::program_option */
    po::variables_map*       _vm;

    /*! @brief the instance of each registered LXCModule */
    std::deque<LXCMCoreModule*>* _modules;
};

#endif /* !LXCMOPTIONS_HH_ */
