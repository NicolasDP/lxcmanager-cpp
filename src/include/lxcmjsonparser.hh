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

#ifndef LXCMJSONPARSER_HH_
# define LXCMJSONPARSER_HH_

# include <istream>

# include "lxcmjson.hh"

class LXCMJsonParser
{
  public:
    LXCMJsonParser ();
    ~LXCMJsonParser ();

//    void parse (char const c);
    void parse (unsigned int const line,
                char const* s, unsigned int const size);
    void parse (unsigned int const line, std::string& s);

    LXCMJsonVal* getDom (void);
  private:
    json_parser _parser;
    json_parser_dom _dom;
};

#endif /* !LXCMJSONPARSER_HH_ */
