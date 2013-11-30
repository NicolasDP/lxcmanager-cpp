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

#ifndef LXCMJSON_HH_
# define LXCMJSON_HH_

# include <cstdint>
# include <map>
# include <deque>
# include <string>
# include <ostream>

# include "json.h"

class LXCMJsonVal
{
  public:
    LXCMJsonVal (int const type);
    LXCMJsonVal (int const type, char const* data, unsigned int const length);

    ~LXCMJsonVal ();

    void print (std::ostream& os) const;

    int _type;

    std::deque<LXCMJsonVal*> _array;
    std::map<std::string, LXCMJsonVal*> _object;
    std::string _data;
};

std::ostream& operator<< (std::ostream& os, LXCMJsonVal const& obj);

void* tree_create_structure (int nesting __attribute__ ((unused)),
                             int is_object);
void* tree_create_data (int type,
                        const char* data, uint32_t length);

int tree_append (void* structure,
                 char* key, uint32_t key_length,
                 void* obj);
#endif /* !LXCMJSON_HH_ */
