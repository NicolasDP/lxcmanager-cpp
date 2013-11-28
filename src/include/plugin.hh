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

class LXCMPlugin : public LXCMModule
{
  public:
    /*! @brief One of LXCMPlugin getter, it returns the version of the
     * current plugin. Useful for program updates, debug and information
     * logs.
     *
     * @return it returns the plugin version (_version)
     */
    int	getVersion ();
    LXCMPlugin (std::string string, int version, PluginTools const* pt)
      : LXCMModule (string)
      , _version (version)
      , _pluginTools (pt)
    {
    };

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

    /*! @Function to send a message */
    virtual int receive (LXCMPlugin*, std::string&) = 0;

  protected:
    int	_version;
    PluginTools const* _pluginTools;
};

#endif /* !PLUGIN_HH_ */
