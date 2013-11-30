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

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <iostream>

#include "lxcmjson.hh"

LXCMJsonVal::LXCMJsonVal (int const type)
  : _type (type)
  , _array (), _object (), _data ()
{
}

LXCMJsonVal::LXCMJsonVal (int const type,
                          char const* data, unsigned int const length)
  : _type (type)
  , _array (), _object (), _data (data, length)
{
}

LXCMJsonVal::~LXCMJsonVal ()
{
  switch (this->_type)
  {
  case JSON_OBJECT_BEGIN:
    {
      std::map<std::string, LXCMJsonVal*>::iterator it;
      std::map<std::string, LXCMJsonVal*>::iterator end;
      end = this->_object.end ();
      for (it = this->_object.begin (); it != end; it++)
      {
        delete it->second;
      }
    }
    break;
  case JSON_ARRAY_BEGIN:
    {
      std::deque<LXCMJsonVal*>::iterator it;
      std::deque<LXCMJsonVal*>::iterator end;
      end = this->_array.end ();
      for (it = this->_array.begin (); it != end; it++)
      {
        delete (*it);
      }
    }
    break;
  default:
    break;
  }
}

void* tree_create_structure (int nesting __attribute__ ((unused)),
                             int is_object)
{
  LXCMJsonVal* val = NULL;
  /* instead of defining a new enum type, we abuse the
   * meaning of the json enum type for array and object */
  if (is_object)
  {
    val = new LXCMJsonVal (JSON_OBJECT_BEGIN);
  }
  else
  {
    val = new LXCMJsonVal (JSON_ARRAY_BEGIN);
  }

  return val;
}

void* tree_create_data (int type,
                        const char* data, uint32_t length)
{
  LXCMJsonVal* val = NULL;

  val = new LXCMJsonVal (type, data, length);

  return val;
}

int tree_append (void* structure,
                 char* key, uint32_t key_length,
                 void* obj)
{
  LXCMJsonVal* parent = (LXCMJsonVal*)structure;

  if (key)
  {
    parent->_object.insert
      (
        std::pair<std::string, LXCMJsonVal*>
          (std::string (key, key_length), (LXCMJsonVal*)obj)
      );
  }
  else
  {
    parent->_array.push_back ((LXCMJsonVal*)obj);
  }

  return 0;
}

void LXCMJsonVal::print (std::ostream& os) const
{
  switch (this->_type)
  {
  case JSON_OBJECT_BEGIN:
    {
      std::map<std::string, LXCMJsonVal*>::const_iterator it;
      std::map<std::string, LXCMJsonVal*>::const_iterator end;
      end = this->_object.cend ();
      os << "Object Begin (" << this->_object.size () << ")" << std::endl;
      for (it = this->_object.begin (); it != end; it++)
      {
	os << "Key (" << it->first << ")" << std::endl;
        os << (*(it->second));
      }
      os << "Object End" << std::endl;
    }
    break;
  case JSON_ARRAY_BEGIN:
    {
      std::deque<LXCMJsonVal*>::const_iterator it;
      std::deque<LXCMJsonVal*>::const_iterator end;
      end = this->_array.cend ();
      os << "Array Begin (" << this->_array.size () << ")" << std::endl;
      for (it = this->_array.cbegin (); it != end; it++)
      {
        os << *(*it);
      }
      os << "Array End" << std::endl;
    }
    break;
  case JSON_FALSE:
    os << "constant: false" << std::endl;;
    break;
  case JSON_TRUE:
    os << "constant: true" << std::endl;;
    break;
  case JSON_NULL:
    os << "constant: null" << std::endl;;
    break;
  case JSON_INT:
    os << "integer: " << this->_data << std::endl;
    break;
  case JSON_STRING:
    os << "string: " << this->_data << std::endl;
    break;
  case JSON_FLOAT:
    os << "float: " << this->_data << std::endl;
    break;
  default:
    break;
  }
}

std::ostream& operator<< (std::ostream& os, LXCMJsonVal const& obj)
{
  obj.print (os);
  return os;
}
