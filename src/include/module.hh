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

/*! @class LXCMModule
 *
 * Most of the part of this project should inherit from this class.
 *
 * A minimal son may be:
 * @code
 * class Foo : public LXCMModule
 * {
 *   public:
 *     Foo ()
 *     {
 *       this->_moduleName = "My name is Foo";
 *     };
 *
 *     ~Foo () {};
 * };
 * @endcode
 */
class LXCMModule
{
  public:
    /*! @brief The moduleName getter, it returns the name of the
     * current module. Useful for debug and information logs.
     *
     * @return it returns the name of the module (_moduleName) */
    virtual std::string& moduleName ()
    {
      return this->_moduleName;
    }

  protected:
    /*! @brief Module name (all module must set this field - use for debug
     * and information log). */
    std::string _moduleName;
};

#endif /* !MODULE_HH_ */
