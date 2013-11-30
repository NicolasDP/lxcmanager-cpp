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
  config.allow_c_comments = 0;
  config.allow_yaml_comments = 1;
  json_parser_init (&this->_parser, &config,
                    json_parser_dom_callback, &this->_dom);
}

LXCMJsonParser::~LXCMJsonParser ()
{
  json_parser_free (&this->_parser);
  json_parser_dom_free (&this->_dom);
}

void LXCMJsonParser::parse (std::string& s)
{
  int ret;

  ret = json_parser_string (&this->_parser, s.c_str (), s.size (), NULL);

  switch (ret)
  {
  case 0:
  case 3:
    break;
  case 7:
    THROW_ERROR (EINVAL, "C-style comment in json not allowed");
    break;
  default:
    std::string msg ("Error with: ");
    msg += s;
    std::cout << "Error code: " << ret << std::endl;
    THROW_ERROR (EBADMSG, msg.c_str ());
    break;
  }
}

void LXCMJsonParser::parseFile (std::string& filename)
{
  std::ifstream ifs;
  std::string word;
  LXCMJsonVal* dom;

  ifs.open (filename.c_str (), std::ifstream::in);

  do
  {
    ifs >> word;
    try
    {
      this->parse (word);
    }
    catch (LXCMException& e)
    {
      THROW_FORWARD_ERROR (e);
    }
  }
  while (ifs.good ());

  dom = (LXCMJsonVal*)this->_dom.root_structure;
  if (dom)
    std::cout << *dom;

  ifs.close ();
}
