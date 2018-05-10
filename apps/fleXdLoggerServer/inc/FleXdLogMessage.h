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
 * File:   FleXdLogMessage.h
 * Author: Jakub Pekar
 */

#ifndef FLEXDLOGMESSAGE_H
#define FLEXDLOGMESSAGE_H

#include <BitStream.h>
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

namespace flexd {
    namespace logger {
        
        class FleXdLogMessage : public BitStream {
        public:
            FleXdLogMessage(std::vector<uint8_t>&& data);
            FleXdLogMessage(uint16_t appID, uint8_t msgType, uint8_t msgCounter, std::string logMessage);
            ~FleXdLogMessage() = default;
             /**
            * Function transforms log in BitStream to the readable form and writes it to the std::cout
            */
            void logToCout();
             /**
            * Function parse the application id from bitstream and return it
            * @return the application ID
            */
            uint16_t getAppID();
             /**
            * Function parse the time of log from bitstream and return it
            * @return the time in epoch form
            */
            uint64_t getTime();
             /**
            * Function parse the message type from bitstream and return it
            * @return type of message in defined form
            */
            uint8_t getMsgType();
             /**
            * Function parse the message counter from bitstream and return it
            * @return count of sending message
            */
            uint8_t getMsgCounter();
             /**
            * Function parse the size of payload from bitstream and return it
            * @return the payload size of message
            */
            uint16_t getMsgSize();
            /**
            * Function parse the message payload from bitstream and return it
            * @return payload
            */
            std::string getLogMessage();
            FleXdLogMessage(const FleXdLogMessage& orig) = delete;
        private:
            static const uint8_t offSet_appID = 0;
            static const uint8_t offSet_time = 16;
            static const uint8_t offSet_msgType = 80;
            static const uint8_t offSet_msgCounter = 88;
            static const uint8_t offSet_msgSize = 96;
            static const uint8_t offSet_logMessage = 112;
            
        };
    } // namespace logger
} // namespace flexd
#endif /* FLEXDLOGMESSAGE_H */

