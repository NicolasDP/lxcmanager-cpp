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

#include "config.hh"
#include "modules.hh"
#include "options.hh"

int main (int argc, char** argv)
{
	int ret;
	OptionsParseCode optPC;
	Options* opts = Options::getOptions ();
	LXCMModules* modules = LXCMModules::getModules ();

	optPC = opts->parseOptions (argc, argv);

	switch (optPC)
	{
	case ERR_NONE:
		ret = 0;
		break;
	case ERR_HELP:
		ret = 0;
		break;
	case ERR_ERROR:
	default:
		ret = 1;
		break;
	}

	modules = modules;

	return ret;
}
