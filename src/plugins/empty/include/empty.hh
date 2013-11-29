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

#ifndef EMPTY_HH_
# define EMPTY_HH_

# include "plugin.hh"
# include "logger.hh"

/*! @class LXCMPEmpty
 * @brief a void plugin: for test and example only */
class LXCMPEmpty : public LXCMPlugin
{
  public:
    /*! @brief The plugin constructor
     * Instanciates the websocket server
     */
    LXCMPEmpty (PluginTools* pt);
    /*! @brief The plugin destructor
     * Deletes the websocket server
     */
    ~LXCMPEmpty ();

  public:
    /*! @brief The mandatory init function
     * Defines the handler when a message is received and initializes
     * the Asio transport system
     */
    void init ();
    /*! @brief The mandatory start function
     * Defines the listening port, the server default acceptance rule and launch
     * the server.
     */
    void start ();
    /*! @brief The mandatory stop function
     * Does nothing for now
     */
    void stop ();
    /*! @brief The mandatory quit function
     * Does nothing for now
     */
    void quit ();

    void receive (LXCMPlugin*, std::string&);

  private:
    bool _lockMessage;
};

#endif /* !EMPTY_HH_ */
