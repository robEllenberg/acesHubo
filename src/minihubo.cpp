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

#include "minihubo.hpp"

namespace MiniHubo{
    AdcHardware::AdcHardware(std::string cfg, std::string args)
      : FlexScanner::Hardware(cfg, args)
        {};

    AdcProtocol::AdcProtocol(std::string cfg, std::string args)
     : FlexScanner::Protocol<adcPacket, adcFlex>(cfg, args)
        {};

    ACES::Credentials* AdcProtocol::credFromPacket(adcPacket* p){
        return new ACES::Credentials(0);
    }

    ACES::Word<float>* AdcDevice::processUS(ACES::Word<adcPacket>* w){
        if(*(w->getCred()) == *credentials){
            int value = w->getData().getValue();

            float realVal = (float)value;

            ACES::Word<float> *res = new ACES::Word<float>(
                realVal /*data*/, w->getData().getChannel() /*nodeID*/,
                0 /*devID*/, ACES::REFRESH, credentials);
            res->printme();
            txUpStream.write(res);
        }
        return NULL;
    }
}
