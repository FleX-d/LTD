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
 * File:   FleXdLogger.cpp
 * Author: Jakub Pekar 
 * Author: Branislav Podkonicky
 */

#include "FleXdLogger.h"
#include "iSocClient.h"
#include "FleXdLogStream.h"
#include <chrono>
#include <iostream>

namespace flexd {
    namespace logger {

        FleXdLogger::FleXdLogger() :
            m_communicator(new iSocClient())
        {   
            m_address ="127.0.0.1";
            m_port = 15000;
            m_flexLogLevel = MsgType::Enum::VERBOSE;
            m_msgCount = 0;
            m_appIDuint = 0;
            m_appName = "";
        }
        
        FleXdLogger& FleXdLogger::instance() {
            static FleXdLogger INSTANCE;
            return INSTANCE;
        }

        void FleXdLogger::initLogger() {
            m_communicator->connectF((char*)m_address.c_str(),m_port);
            handshake();
        }

        void FleXdLogger::setAppName(const std::string appName) {
            if (!this->m_appName.compare("")){
                this->m_appName = appName;
                initLogger();
            }
        }
        
        void FleXdLogger::sendLog(const MsgType::Enum logLevel, const std::stringstream&& stream, time_t time){          
            if (logLevel <= m_flexLogLevel){  
                bool decreaseCounter =  false;
                if (!m_appName.compare("")){
                    this->setAppName(randomString(size_t(8)));
                    decreaseCounter = true;
                }
               
                LogStream logStream(m_appIDuint, time, m_flexLogLevel, m_msgCount, std::move(stream));
                std::vector<uint8_t> streamVector = logStream.releaseData();
                m_communicator->send(streamVector.data(), streamVector.size());
                m_msgCount++;
                if (decreaseCounter) {
                    m_msgCount--;
                }
            }
        }
        
        std::string FleXdLogger::randomString( size_t length ) const
        {
            auto randchar = []() -> char
        {
            const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
            const size_t max_index = (sizeof(charset) - 1);
            return charset[ rand() % max_index ];
        };
            std::string str(length,0);
            std::generate_n( str.begin(), length, randchar );
            return str;
        }
        
        void FleXdLogger::handshake() {
            uint64_t time = 
                std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            time_t endwait;
            time_t start = time_t(NULL);
            time_t seconds = 10; // end loop aftFlexLoggerer this time has elapsed
            endwait = start + seconds;
           
            char buffer[1024];
            
            LogStream handshakeMessage(0, (uint64_t)time, flexd::logger::MsgType::HANDSHAKE, 0, m_appName);
            std::vector<uint8_t> streamVector = handshakeMessage.releaseData();
            m_communicator->send(streamVector.data(), streamVector.size());
            
            int valread = m_communicator->recv(buffer, 1024);
            std::vector<uint8_t> dataBuffer(buffer, buffer + valread);
            std::cout << "Valread " << valread << std::endl;        
            while (start < endwait)
            {
                start = time_t(NULL);

                if (valread) {
                    LogStream responseStream(std::move(dataBuffer)); //create log response message
                    responseStream.logToCout();
            
                    uint8_t msgTpe = responseStream.getMessageType();
                    
                    if (msgTpe == MsgType::Enum::HANDSHAKESUCCES) {
                        this->m_appIDuint = responseStream.getAppID();
                        break;
                    } else if (msgTpe == MsgType::Enum::HANDSHAKEFAIL) {
                        this->setAppName(randomString(size_t(8)));
                    }   
                }        
            }          
        }  

    } // namespace FlexLogger
} // namespace flexd

