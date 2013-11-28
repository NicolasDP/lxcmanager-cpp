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

#ifndef PLUGMODULES_HH_
# define PLUGMODULES_HH_

# include <map>
# include <string>

# include "options.hh"

# include "plugin.hh"

/*! @class LXCMPlugModules
 *
 * TODO */
class LXCMPlugModules : public LXCMCoreModule
{
  public:
    /*! @brief destructor */
    ~LXCMPlugModules (void);
  private:
    /*! @brief constructor */
    LXCMPlugModules (void);

  public:
    /*! @brief init it initializes the unique instance of this singleton */
    static void init (void);
    /*! @brief load modules from the default path or from the directory given
     * in command line option.
     * @return 0 on success or 1 on error */
    static int loadModules (void);
    /*! @brief checkOptions
     * it implements the virtual method defined in the
     * LXCMCoreModule. */
    OptionsParseCode checkOptions (po::variables_map&);

    static int sendMessage (LXCMPlugin*, std::string&, std::string&);

  private:
    /*! @brief load modules from the path given in parameter
     * @return 0 on success or 1 on error */
    int exploreDir (std::string&);
    int sendMessageToPlugin (LXCMPlugin*, std::string&, std::string&);
  private:
    /*! @brief the unique instant of this singleton */
    static LXCMPlugModules* _instance;

  private:
    std::map<std::string, void*> _libs;
    std::map<std::string, LXCMPlugin*> _modules;
    std::string _modulesDirectory;
};

#endif /* !PLUGMODULES_HH_ */
