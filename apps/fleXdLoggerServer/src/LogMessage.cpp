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
 * File:   LogMessage.cpp
 * Author: Jakub Pekar
 */

#include <sstream>
#include <string>
#include <syslog.h>
#include <deque>
#include "LogMessage.h"

namespace flexd {
    namespace FlexLogger {

        /*Creating of logMessage from bitstream*/
        LogMessage::LogMessage(std::vector<uint8_t>&& data)
        : BiteStream(std::move(data)) {
        }

        LogMessage::LogMessage(uint16_t appID, uint8_t msgType, uint8_t msgCounter, std::string logMessage) {
            const std::time_t time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            uint16_t sizeOfPayload = logMessage.length();
            this->put(appID, 16);
            this->put(time, 64);
            this->put(msgType, 8);
            this->put(msgCounter, 8);
            this->put(sizeOfPayload, 16);
            this->put(logMessage.begin(), logMessage.end());
        }

        void LogMessage::logToCout() {
            std::cout << "AppID: " << std::to_string(getAppID())
                    << " Time: " << std::to_string(getTime())
                    << " msgType " << std::to_string(getMsgType())
                    << " msgCounter " << std::to_string(getMsgCounter())
                    << " msgLength " << std::to_string(getMsgSize())
                    << " MSG: " << getLogMessage()
                    << std::endl;
        }

        bool LogMessage::logToSysLog(std::string name) {
            std::string priority;
            uint8_t sysLogType = 4;
            switch (getMsgType()) {
                case 0:priority = "HANDSHAKE";
                    //sysLogType = 6;
                    break;
                case 1:priority = "VERBOSE";
                    //sysLogType = 6;
                    break;
                case 2:priority = "DEBUG";
                    // sysLogType = 7;
                    break;
                case 3:priority = "INFO";
                    //sysLogType = 6;
                    break;
                case 4:priority = "WARN";
                    //sysLogType = 5;
                    break;
                case 5:priority = "ERROR";
                    //sysLogType = 3;
                    break;
                case 6:priority = "FATAL";
                    //sysLogType = 2;
                    break;
                case 7:priority = "ALL";
                    //sysLogType = 1;
                    break;
                default:
                    priority = "*unknow*";
                    break;
            }
            syslog(sysLogType, "[%s][%ld][%s] %s ", name.c_str(), \
                    getTime(),\
                    priority.c_str(),\
                    getLogMessage().c_str()
                    );
            return true;
        }

        uint16_t LogMessage::getAppID() {
            return this->getWithOffset<uint16_t>(0, sizeof (uint16_t)*8);
        }

        uint64_t LogMessage::getTime() {
            return getWithOffset<uint64_t>(offSet_time, sizeof (uint64_t)*8);
        }

        uint8_t LogMessage::getMsgType() {
            return getWithOffset<uint8_t>(offSet_msgType, sizeof (uint8_t)*8);
        }

        uint8_t LogMessage::getMsgCounter() {
            return getWithOffset<uint8_t>(offSet_msgCounter, sizeof (uint8_t)*8);
        }

        uint16_t LogMessage::getMsgSize() {
            return getWithOffset<uint16_t>(offSet_msgSize, sizeof (uint16_t)*8);
        }

        std::string LogMessage::getLogMessage() {
            uint16_t msgSize = getMsgSize();
            return std::string(this->m_data.begin() + 14, this->m_data.begin() + 14 + msgSize);
        }
    } // namespace FlexLogger
} // namespace flexd
