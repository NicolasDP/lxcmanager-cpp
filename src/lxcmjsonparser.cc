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

#include "lxcmjsonparser.hh"
#include "exceptions.hh"
#include <fstream>
#include <iostream>

LXCMJsonParser::LXCMJsonParser ()
{
  json_parser_dom_init (&this->_dom, tree_create_structure,
                        tree_create_data, tree_append);

  json_config config;
  memset (&config, 0, sizeof (config));
  config.max_nesting = 0;
  config.max_data = 0;
  config.allow_c_comments = 1;
  config.allow_yaml_comments = 0;
  json_parser_init (&this->_parser, &config,
                    json_parser_dom_callback, &this->_dom);
}

LXCMJsonParser::~LXCMJsonParser ()
{
  json_parser_free (&this->_parser);
  json_parser_dom_free (&this->_dom);
}

void LXCMJsonParser::parse (unsigned int const line,
                            char const* s, unsigned int const size)
{
  int ret;
  unsigned int col = 0;

  ret = json_parser_string (&this->_parser, s, size, &col);

  switch (ret)
  {
  case 0:
  case 3:
    break;
  case 7:
    THROW_ERROR (EINVAL, "yaml-style comment in json not allowed");
    break;
  default:
    char msg[256];
    memset (msg, 0, sizeof (msg));
    snprintf (msg, sizeof (msg),
              "Error line (%u) caracter (%u) line (%s)", line, col, s);
    std::cout << "Error code: " << ret << std::endl;
    THROW_ERROR (EINVAL, msg);
    break;
  }
}

LXCMJsonVal* LXCMJsonParser::getDom (void)
{
  return (LXCMJsonVal*)this->_dom.root_structure;
}

void LXCMJsonParser::parse (unsigned int const line, std::string& s)
{
  this->parse (line, s.c_str (), s.size ());
}
