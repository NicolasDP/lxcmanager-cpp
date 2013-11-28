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

#ifndef LXCMVERSION_HH_
# define LXCMVERSION_HH_

# include "options.hh"
# include "config.hh"

/*!
 * @class LXCMVersion
 *
 * This Class runs in stand-alone, it just exposes interfaces to get
 * version information about the main program. */
class LXCMVersion : public LXCMCoreModule
{
  public:
    /*!
     * @brief init
     * As LXCMVersion is a singleton design pattern, this is the
     * main function to initialize the unique instance of this
     * Class. */
    static void init ();
    /*! @brief Destructor */
    ~LXCMVersion ();

    /*!
     * @brief checkOptions
     * it implements the virtual method defined in the
     * LXCMCoreModule. */
     void checkOptions (po::variables_map&);

  private:
    /*! @brief Constructor */
    LXCMVersion ();
    /*!< @brief the unique instance of this Class */
    static LXCMVersion* _singleton;
};

#endif /* !LXCMVERSION_HH_ */
