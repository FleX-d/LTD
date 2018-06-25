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
 * File:   FleXdLogger.cpp
 * Author: Jakub Pekar
 * Author: Branislav Podkonicky
 */

#include "FleXdLogger.h"
#include "FleXdLogStream.h"
#include "FleXdMessageType.h"
#include "FleXdLoggerIPCClient.h"

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

    flexd::logger::LogLevel::Enum msgTypeToLogLevel(flexd::logger::MsgType::Enum msgType) {
        switch (msgType) {
            case flexd::logger::MsgType::Enum::VERBOSE:
                return flexd::logger::LogLevel::Enum::VERBOSE;
            case flexd::logger::MsgType::Enum::DEBUG:
                return flexd::logger::LogLevel::Enum::DEBUG;
            case flexd::logger::MsgType::Enum::INFO:
                return flexd::logger::LogLevel::Enum::INFO;
            case flexd::logger::MsgType::Enum::WARN:
                return flexd::logger::LogLevel::Enum::WARN;
            case flexd::logger::MsgType::Enum::ERROR:
                return flexd::logger::LogLevel::Enum::ERROR;
            case flexd::logger::MsgType::Enum::FATAL:
                return flexd::logger::LogLevel::Enum::FATAL;
            default:
                //TODO: default value
                return flexd::logger::LogLevel::Enum::ERROR;
        }
    }
}

namespace flexd {
    namespace logger {

        FleXdLogger::FleXdLogger()
        : m_connectionToServer(false),
          m_appName(""),
          m_appIDuint(0),
          m_logBuffer(new FleXdLogBuffer(FLEXDLOGGER_MAXLOGBUFFERSIZE)),
          m_IPCClient(nullptr),
          m_msgCount(0) {
        }

        FleXdLogger::~FleXdLogger() {
        }

        FleXdLogger& FleXdLogger::instance()
        {
            static FleXdLogger INSTANCE;
            return INSTANCE;
        }

        bool FleXdLogger::loggerInit(flexd::icl::ipc::FleXdEpoll& poller, const std::string& appName) {
            m_IPCClient = std::make_unique<FleXdLoggerIPCClient>(m_logBuffer, poller);
            m_IPCClient->setName(appName);
            if (m_IPCClient->init()) {
                handshake();
                return true;
            } else {
                //TODO
                std::cout << "FleXdLogger::loggerInit() failed" << std::endl;
            }
            return false;
        }

        void FleXdLogger::writeLog(LogLevel::Enum logLevel, std::time_t time, const std::string& level, const std::string& stream) {
            if(msgTypeToLogLevel(m_IPCClient->getLogLvlFilter()) <= logLevel){
                writeLogToBuffer(logLevel, stream, time);
                if (m_IPCClient && !m_IPCClient->isConnected()) {
                std::cout << "FleXdLogger::[" << m_IPCClient->getName() << "][" << m_IPCClient->getAppID() << "][" << time <<"][" << level << "] : " << stream << std::endl;
                }
            }
        }

        void FleXdLogger::writeLogToBuffer(const LogLevel::Enum logLevel, const std::string& stream, time_t time) {
                m_logBuffer->push(LogData(time, logLevelToMsgType(logLevel), m_msgCount, stream));
                m_msgCount++;
                if (m_IPCClient) m_IPCClient->flushBuffer();

        }

        void FleXdLogger::handshake() {
            if (m_IPCClient) {
                FleXdLogStream handshakeMessage(
                    0, flexd::logger::MsgType::HANDSHAKE, 0, m_IPCClient->getName(),
                    (uint64_t) std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
                m_IPCClient->sndMsg(std::make_shared<flexd::icl::ipc::FleXdIPCMsg>(flexd::logger::MsgType::HANDSHAKE, std::move(handshakeMessage.releaseData())));
            }
        }

    } // namespace FlexLogger
} // namespace flexd

