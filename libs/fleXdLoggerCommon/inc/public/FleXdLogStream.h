/*
Copyright (c) 2018, Globallogic s.r.o.
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
 * Author: Jakub Pekar
 *
 * Created on June 5, 2018, 12:44 PM
 */

#ifndef FLEXDLOGSTREAM_H
#define FLEXDLOGSTREAM_H

#include <BitStream.h>
#include <iostream>
#include <chrono>
#include "FleXdMessageType.h"

#define OFFSET_APPID 0
#define OFFSET_TIME 16
#define OFFSET_MSGTYPE 80
#define OFFSET_MSGCOUNTER 88
#define OFFSET_MSGSIZE 96
#define OFFSET_LOGMSG 112
#define APPID_SIZE 16
#define TIME_SIZE 64
#define MSGTYPE_SIZE 8
#define MSGCOUNTER_SIZE 8
#define MSGLENGTH_SIZE 16

namespace flexd {
    namespace logger {

        class FleXdLogStream : public BitStream {
        public:
            explicit FleXdLogStream(std::vector<uint8_t>&& data) : BitStream(std::move(data)) {}
            explicit FleXdLogStream(uint16_t appID, uint8_t msgType, uint8_t msgCounter, const std::string& message, uint64_t time = 0) {
                BitStream::put(appID, APPID_SIZE);
                BitStream::put(time != 0 ? time : (uint64_t) std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(), TIME_SIZE);
                BitStream::put(msgType, MSGTYPE_SIZE);
                BitStream::put(msgCounter, MSGCOUNTER_SIZE);
                BitStream::put((uint16_t) message.length(), MSGLENGTH_SIZE);
                BitStream::put(message.begin(), message.end());
            }
            FleXdLogStream(const FleXdLogStream&) = delete;
            FleXdLogStream& operator=(const FleXdLogStream&) = delete;
            FleXdLogStream(FleXdLogStream&& stream) : BitStream(std::move(stream)) {}
            FleXdLogStream& operator=(FleXdLogStream&&) = delete;
            virtual ~FleXdLogStream() = default;
            /*
             * Function parses the appID from BitStream
             * @return appID from stream
             */
            uint16_t getAppID() { return BitStream::getWithOffset<uint16_t>(0, sizeof(uint16_t)*8); }
            /*
             * Function parses the time of log from BitStream
             * @return appID from stream
             */
            uint64_t getTime() { return BitStream::getWithOffset<uint64_t>(OFFSET_TIME, sizeof(uint64_t)*8); }
            /*
             * Function parses the message type from BitStream
             * @return message type from stream
             */
            uint8_t getMsgType() { return BitStream::getWithOffset<uint8_t>(OFFSET_MSGTYPE, sizeof(uint8_t)*8); }
            /*
             * Function parses the message counter from BitStream
             * @return message counter from stream
             */
            uint8_t getMsgCounter() { return BitStream::getWithOffset<uint8_t>(OFFSET_MSGCOUNTER, sizeof(uint8_t)*8); }
            /*
             * Function parses the payload length from BitStream
             * @return payload length from stream
             */
            uint16_t getMsgSize() { return BitStream::getWithOffset<uint16_t>(OFFSET_MSGSIZE, sizeof(uint16_t)*8); }
            /*
             * Function parses payload from BitStream
             * @return payload from message
             */
            std::string getMessage() {
                getMsgSize();
                std::vector<uint8_t> data = BitStream::getRest();
                return std::string(data.begin(), data.end());
            }
            /*
             * Function returns header size
             * @return header size
             */
            static uint8_t getHeadersize() { return APPID_SIZE + TIME_SIZE + MSGTYPE_SIZE + MSGCOUNTER_SIZE + MSGLENGTH_SIZE; }
            /*
             * Function parse message from BitStream to readable form and prints log to console
             */
            void logToCout() {
                std::cout << "AppID: " << std::to_string(getAppID())
                        << " Time: " << std::to_string(getTime())
                        << " msgType " << std::to_string(getMsgType())
                        << " msgCounter " << std::to_string(getMsgCounter())
                        << " msgLength " << std::to_string(getMsgSize())
                        << " MSG: " << getMessage()
                        << std::endl;
            }
        };

    } // namespace logger
} // namespace flexd

#endif /* FLEXDLOGSTREAM_H */

