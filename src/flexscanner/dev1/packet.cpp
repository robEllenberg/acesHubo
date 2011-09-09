/* 
    Conductor: High Degree of Freedom Robot Controller Framework
    Copyright (C) 2010, 2011 Robert Sherbert
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
    please contact the author. The software can be licensed to you under
    non-free license terms by the copyright holder.

    As a courtesy to the author, and in the spirit of fair attribution, you are
    asked to cite the following paper if any work based upon or utilizing this
    framework is published in the scientific literature: 
    Sherbert, Robert M. and Oh, Paul Y. "Conductor: A Controller Development
    Framework for High Degree of Freedom Systems." Intelligent Robots and
    Systems (IROS), 2011 IEEE/RSJ International Conference on. 
*/

#include "packet.hpp"

void packet::printme(){
    std::cout << "id: " << std::hex << (int) this->id << ", ";
    std::cout << "len: " << std::hex << (int) this->len << ", ";
    std::cout << "error: " << std::hex << (int) this->error << ", ";
    std::cout << "check: " << std::hex << (int) this->checksum << ", ";
    std::cout << "param: " << parameters << ", ";
    std::cout << std::endl;
}

packet::packet(){
    id = 0;
    len = 0;
    error = 0;
    checksum = 0;
    parameters = new std::list<unsigned char>;
}
