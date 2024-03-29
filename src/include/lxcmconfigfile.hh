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

#ifndef LXCMConfigFile_HH_
# define LXCMConfigFile_HH_

# include "options.hh"

# include "lxcmjson.hh"

/*!
 * @class LXCMConfigFile
 *
 * TODO*/
class LXCMConfigFile : public LXCMCoreModule
{
  public:
    /*! @brief init
     * As LXCMConfigFile is a singleton design pattern, this is the
     * main function to initialize the unique instance of this
     * Class. */
    static void init ();
    /*! @brief Destructor */
    ~LXCMConfigFile ();

    /*! @brief checkOptions
     * it implements the virtual method defined in the
     * LXCMCoreModule. */
     void checkOptions (po::variables_map&);

     /*! @brief print the Tree
      * @param os the output stream */
     void print (std::ostream& os) const;

     static LXCMJsonVal const* getModuleOptions (LXCMModule const* module);

  private:
    /*! @brief Constructor */
    LXCMConfigFile ();
    /*!< @brief the unique instance of this Class */
    static LXCMConfigFile* _instance;

  private:
    LXCMJsonVal* _dom;
};

LXCMJsonVal const* lxcm_configfile_get_my_config (LXCMModule const* module);
std::ostream& operator<< (std::ostream& os, LXCMConfigFile const& obj);

#endif /* !LXCMConfigFile_HH_ */
