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

#ifndef MODULES_HH_
# define MODULES_HH_

# include <map>
# include <string>

# include "options.hh"

class LXCMModules : public LXCMCoreModule
{
	public:
		~LXCMModules (void);
	private:
		LXCMModules (void);

	public:
		static LXCMModules* init (void);
		OptionsParseCode checkOptions (po::variables_map&);

	private:
		static LXCMModules* _modules;
};

#endif /* !MODULES_HH_ */
