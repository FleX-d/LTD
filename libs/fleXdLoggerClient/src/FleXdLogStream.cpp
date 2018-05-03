/*
Copyright (c) 2017, Globallogic s.r.o.
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
 * Neither the name of the Globallogic s.r.o. nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC S.R.O. BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* 
 * File:   FleXdLogStream.cpp
 * Author: Branislav Podkonicky
 *
 * Created on March 15, 2018, 12:44 PM
 */

#include "FleXdLogStream.h"
#include <iostream>
#include <FleXdMessageType.h>


namespace flexd {
    namespace logger {

        LogStream::LogStream(std::vector<uint8_t>&& data) : BitStream(std::move(data)) {
        }

        LogStream::LogStream(uint16_t appID, time_t time, flexd::logger::MsgType::Enum messageType, uint8_t messageCounter, const std::string& message) {
            uint16_t messageLength = message.length();
            this->put(appID, appID_size);
            this->put(time, time_size);
            this->put((uint8_t) messageType, messageType_size);
            this->put(messageCounter, messageCounter_size);
            this->put(messageLength, messageLength_size);
            this->put(message.begin(), message.end());
        }

        LogStream::LogStream(uint16_t appID, time_t time, flexd::logger::MsgType::Enum messageType, uint8_t messageCounter, const std::stringstream&& message) {
            uint16_t messageLength = message.str().length();
            this->put(appID, appID_size);
            this->put(time, time_size);
            this->put((uint8_t) messageType, messageType_size);
            this->put(messageCounter, messageCounter_size);
            this->put(messageLength, messageLength_size);
            std::string msg = message.str(); // TODO avoid copy stream
            this->put(msg.begin(), msg.end());
        }

        LogStream::~LogStream() {
        }

        uint16_t LogStream::getAppID() {
            return this->getWithOffset<uint16_t>(0, appID_size);
        }

        time_t LogStream::getTime() {
            return this->getWithOffset<time_t>(appID_size, time_size);
        }

        flexd::logger::MsgType::Enum LogStream::getMessageType() {
            uint8_t returnValue = this->getWithOffset<uint8_t>(appID_size + time_size, messageType_size);
            return (flexd::logger::MsgType::Enum) returnValue;
        }

        uint8_t LogStream::getMessageCounter() {
            return this->getWithOffset<uint8_t>(appID_size + time_size + messageType_size, messageCounter_size);
        }

        uint16_t LogStream::getMessageLength() {
            return this->getWithOffset<uint16_t>(appID_size + time_size + messageType_size + messageCounter_size, messageLength_size);
        }

        std::string LogStream::getMessage() {
            getMessageLength();
            std::vector<uint8_t> data = this->getRest();
            return std::string(data.begin(), data.end());
        }

        void LogStream::logToCout() {
            std::cout << "AppID: " << std::to_string(getAppID())
                    << " Time: " << std::to_string(getTime())
                    << " msgType: " << std::to_string(getMessageType())
                    << " msgCounter: " << std::to_string(getMessageCounter())
                    << " msgLength: " << std::to_string(getMessageLength())
                    << " MSG: " << getMessage()
                    << std::endl;
        }
    } // namespace logger
} // namespace flexd
