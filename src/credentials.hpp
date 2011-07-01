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

#ifndef ACES_CREDENTIALS_HPP
#define ACES_CREDENTIALS_HPP

#include <rtt/Logger.hpp>

namespace ACES {

    class Credentials {
        public:
            Credentials(int id);
            Credentials(std::string args);
            virtual void printme();
            virtual bool operator==(Credentials& other);
            int getDevID();
            static Credentials* makeCredentials(std::string args);
            static Credentials* makeCredentials(int newid);
        protected:
            //! Unique numerical identifier for device
            /*! The unique ID for the device is used to match upstream packets
             * to the owner device. The physical meaning of this is dependent
             * on the underlying hardware. (Likely a device ID of some sort).
             */
            int devID;  
                        
    };
}
#endif
