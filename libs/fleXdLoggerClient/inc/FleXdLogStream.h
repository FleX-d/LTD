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
 * File:   FleXdLogStream.h
 * Author: Branislav Podkonicky
 *
 * Created on March 15, 2018, 12:44 PM
 */

#ifndef FLEXDLOGSTREAM_H
#define FLEXDLOGSTREAM_H

#include <BitStream.h>
#include <sstream>
#include "FleXdMessageType.h"

namespace flexd {
    namespace logger {

        class LogStream : public BitStream {
        public:
            explicit LogStream(std::vector<uint8_t>&& data);
            explicit LogStream(uint16_t appID, time_t time, flexd::logger::MsgType::Enum messageType, uint8_t messageCounter, const std::string& message);
            explicit LogStream(uint16_t appID, time_t time, flexd::logger::MsgType::Enum messageType, uint8_t messageCounter, const std::stringstream&& message);
            virtual ~LogStream();
            uint16_t getAppID();
            time_t getTime();
            flexd::logger::MsgType::Enum getMessageType();
            uint8_t getMessageCounter();
            uint16_t getMessageLength();
            std::string getMessage();
            virtual void logToCout();

        private:
            const static size_t appID_size = 16;
            const static size_t time_size = 64;
            const static size_t messageType_size = 8;
            const static size_t messageCounter_size = 8;
            const static size_t messageLength_size = 16;
        };
    } // namespace logger
} // namespace flexd

#endif /* FLEXDLOGSTREAM_H */

