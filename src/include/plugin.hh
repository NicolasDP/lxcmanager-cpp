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

#ifndef PLUGIN_HH_
# define PLUGIN_HH_

# include "module.hh"
# include "plugintools.hh"

/*! @class LXCMPlugin
 *
 * @brief This is the Class that all plugin Must inherit. */
class LXCMPlugin : public LXCMModule
{
  public:
    /*! @brief constructor
     *
     * @param string is the plugin's name
     * @param version is the plugin's version
     * @param pt is the tools functions given by the core */
    LXCMPlugin (std::string string, int version, PluginTools const* pt)
      : LXCMModule (string)
      , _version (version)
      , _pluginTools (pt)
    {
    };
    /*! @brief One of LXCMPlugin getter, it returns the version of the
     * current plugin. Useful for program updates, debug and information
     * logs.
     *
     * @return it returns the plugin version (_version) */
    int	getVersion ();

  public:
    /*! @brief virtual interface that child must implement
     * It is use to init a plugin
     * If there is no initialization to handle, then this function does
     * nothing but must be implemented.
     */
    virtual void init () = 0;
    /*! @brief virtual interface that child must implement
     * It is use to start a plugin, the plugin should be autonomous
     * after this function is called.
     */
    virtual void start () = 0;
    /*! @brief virtual interface that child must implement
     * It is use to stop a plugin
     */
    virtual void stop () = 0;
    /*! @brief virtual interface that child must implement
     * It is use to exit a plugin
     * If there is nothing to do, then this function does
     * nothing but must be implemented.
     */
    virtual void quit () = 0;

    /*! @brief to send a message
     * @param from is the pointer to the sender address (this)
     * @param msg is the message */
    virtual void receive (LXCMPlugin* from, std::string& msg) = 0;

  protected:
    int	_version; /*!< the version number */
    PluginTools const* _pluginTools; /*!< the given tools */
};

#endif /* !PLUGIN_HH_ */
