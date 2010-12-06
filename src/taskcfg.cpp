/* AUTORIGHTS
    ACES: Abstracted Controls Engineering Software
    Copyright (C) 2010 Robert Sherbert
    bob.sherbert@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    If you are interested in licensing this software for commercial purposes
    please contact the author.
*/

#include "taskcfg.hpp"
namespace ACES{

    taskCfg::taskCfg(){
    name = "";
    priority = 10;
    freq = 1;
    }

    taskCfg::taskCfg(std::string cfg){
        std::istringstream s1(cfg, std::istringstream::in);
        s1 >> name;
        s1 >> priority;
        s1 >> freq;
    }

    std::string taskCfg::Name(){
        return name;
    }
}
