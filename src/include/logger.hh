#ifndef LOGGER_HH_
# define LOGGER_HH_

# define NOTICE = 0
# define ERROR = 1

# include <iostream>
# include <iomanip>
# include <string>

# include "options.hh"

class Logger : public LXCMModule
{
  public:
    static Logger* getLogger ();
    ~Logger ();

    void Log (std::string, int, int);
    OptionsParseCode checkOptions (po::variables_map&);

  private:
    Logger ();
    static Logger* _singleton;

  private:
    void LogError (std::string, int);
    void LogStandard (std::string, int);

  private:
    static const int _maxLevel = 1;
};

#endif /* !LOGGER_HH_ */
