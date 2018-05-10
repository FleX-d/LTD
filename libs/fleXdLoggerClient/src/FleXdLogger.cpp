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
#include "FleXdMessageType.h"
#include "FleXdLogBuffer.h"
#include <chrono>
#include <iostream>

namespace {

    flexd::logger::MsgType::Enum logLevelToMsgType(flexd::logger::LogLevel::Enum logLevel) {
        switch (logLevel) {
            case flexd::logger::LogLevel::Enum::VERBOSE:
                return flexd::logger::MsgType::Enum::VERBOSE;
            case flexd::logger::LogLevel::Enum::DEBUG:
                return flexd::logger::MsgType::Enum::DEBUG;
            case flexd::logger::LogLevel::Enum::INFO:
                return flexd::logger::MsgType::Enum::INFO;
            case flexd::logger::LogLevel::Enum::WARN:
                return flexd::logger::MsgType::Enum::WARN;
            case flexd::logger::LogLevel::Enum::ERROR:
                return flexd::logger::MsgType::Enum::ERROR;
            case flexd::logger::LogLevel::Enum::FATAL:
                return flexd::logger::MsgType::Enum::FATAL;
            default:
                //TODO: default value
                return flexd::logger::MsgType::Enum::ERROR;
        }
    }
}

namespace flexd {
    namespace logger {


        FleXdLogger::FleXdLogger()
        : m_connectionToServer(false),
        m_appName(""),
        m_appIDuint(0),
        m_flexLogLevel(LogLevel::Enum::VERBOSE),
        m_communicator(new iSocClient()),
        m_logBuffer(new FleXdLogBuffer(2048)),         
        m_msgCount(0)       
        {
            //m_logBuffer = new FleXdLogBuffer(2048);
            
        }
        
        FleXdLogger::~FleXdLogger(){
            //delete m_logBuffer;
        }

        FleXdLogger& FleXdLogger::instance() 
        {
            static FleXdLogger INSTANCE;
            return INSTANCE;
        }

        void FleXdLogger::initLogger() 
        {
            if(!(m_communicator->connectF())){
                m_connectionToServer = false;
            } else {
                if(handshake()){
                    m_connectionToServer = true;
                    std::cout <<  "FleXdLogger::->  Logging to ServerLogger." << std::endl; 
                } else {                                   
                    m_connectionToServer = false;
                }
            }
        }

        void FleXdLogger::setAppName(const std::string appName) {
            if (!this->m_appName.compare("")) {
                this->m_appName = appName;
                initLogger();
            }
        }
        
        void FleXdLogger::writeLogToBuffer(const LogLevel::Enum logLevel, const std::stringstream&& stream, time_t time) {
            if (logLevel >= m_flexLogLevel) {
                LogStream logStream(m_appIDuint, time, logLevelToMsgType(logLevel), m_msgCount, std::move(stream));
                m_logBuffer->push(std::move(logStream));
                m_msgCount++;
                sendLog();
            }
        }
        
        bool FleXdLogger::sendLog() {
            bool freeSocketQueue = true;
            bool returnValue = false;
            while(freeSocketQueue && (m_logBuffer->getSizeBuffer() > 0)){           // loop that sending logs from queue, while queue is not free or socket buffer full
                LogStream& logStreamSend = m_logBuffer->getStream();
                logStreamSend.reset();
                std::vector<uint8_t> streamVector = logStreamSend.getData();
                if(m_communicator->send(streamVector.data(), streamVector.size()) > 0){
                    m_logBuffer->pop();
                    returnValue = true;
                } else {
                    //TODO comparing errno if is socket buffer full
                    freeSocketQueue = false;
                    returnValue = false;
                }
            }
            return returnValue;
        }

        std::string FleXdLogger::randomString(size_t length) const {
            auto randchar = []() -> char {
                const char charset[] =
                        "0123456789"
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                        "abcdefghijklmnopqrstuvwxyz";
                const size_t max_index = (sizeof (charset) - 1);
                return charset[ rand() % max_index ];
            };
            std::string str(length, 0);
            std::generate_n(str.begin(), length, randchar);
            return str;
        }

        bool FleXdLogger::handshake() {
            uint64_t time =
                    std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            char buffer[1024];

            LogStream handshakeMessage(0, (uint64_t) time, flexd::logger::MsgType::HANDSHAKE, 0, m_appName);
            std::vector<uint8_t> streamVector = handshakeMessage.releaseData();
            m_communicator->send(streamVector.data(), streamVector.size());

            int valread = m_communicator->recv(buffer, 1024);
            std::vector<uint8_t> dataBuffer(buffer, buffer + valread);
            if (valread) {
                LogStream responseStream(std::move(dataBuffer)); //create log response message
                

                uint8_t msgTpe = responseStream.getMessageType();

                if (msgTpe == MsgType::Enum::HANDSHAKESUCCES) {
                    this->m_appIDuint = responseStream.getAppID();
                    responseStream.logToCout();
                    return true;
                } else if (msgTpe == MsgType::Enum::HANDSHAKEFAIL) {
                    m_communicator->closeSocket();
                    this->setAppName(randomString(size_t(8)));
                    responseStream.logToCout();
                    
                }
            } else {
                //TODO if unconnected socket it return false
                return false;
            }
            return false;
        }

    } // namespace FlexLogger
} // namespace flexd

