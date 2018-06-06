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
 * File:   FleXdLogger.h
 * Author: Jakub Pekar
 * Author: Branislav Podkonicky
 */

#ifndef FLEXDLOGGER_H
#define FLEXDLOGGER_H

#include <algorithm>
#include <thread>
#include <sstream>
#include <iostream>

#define FLEX_LOG_TRACE(...)  \
    flexd::logger::FleXdLogger::instance().log(flexd::logger::LogLevel::Enum::VERBOSE, "verbose", std::this_thread::get_id(), std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(), __VA_ARGS__)
#define FLEX_LOG_DEBUG(...)  \
    flexd::logger::FleXdLogger::instance().log(flexd::logger::LogLevel::Enum::DEBUG, "debug", std::this_thread::get_id(), std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(), __VA_ARGS__)
#define FLEX_LOG_INFO(...)  \
    flexd::logger::FleXdLogger::instance().log(flexd::logger::LogLevel::Enum::INFO, "info", std::this_thread::get_id(), std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(), __VA_ARGS__)
#define FLEX_LOG_WARN(...)  \
    flexd::logger::FleXdLogger::instance().log(flexd::logger::LogLevel::Enum::WARN, "warning", std::this_thread::get_id(), std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(), __VA_ARGS__)
#define FLEX_LOG_ERROR(...)  \
    flexd::logger::FleXdLogger::instance().log(flexd::logger::LogLevel::Enum::ERROR, "error", std::this_thread::get_id(), std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(), __VA_ARGS__)
#define FLEX_LOG_FATAL(...)  \
    flexd::logger::FleXdLogger::instance().log(flexd::logger::LogLevel::Enum::FATAL, "fatal", std::this_thread::get_id(), std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(), __VA_ARGS__)
#define FLEX_LOG_INIT( ...)  \
    flexd::logger::FleXdLogger::instance().loggerInit(__VA_ARGS__);
#define FLEX_START_LOOP(...) \
    flexd::logger::FleXdLogger::instance().runLoop(__VA_ARGS__);

#define FLEXDLOGGER_MAXLOGBUFFERSIZE 2048

namespace flexd {
    namespace icl {
        namespace ipc {
            class FleXdEpoll;
        }
    }
}

namespace flexd {
    namespace logger {

        class FleXdLogBuffer;
        class FleXdLoggerIPCClient;

    } //namespace logger
} //namespace flexd

namespace {

    template<typename T_head>
    void FleXdLog(std::stringstream& ss, T_head& head) {
        ss << head;
    }

    template<typename T_head, typename... T_tail>
    void FleXdLog(std::stringstream& ss, T_head& head, T_tail&... tail) {
        FleXdLog(ss, head);
        FleXdLog(ss, tail...);
    }

    template<>
    void FleXdLog<uint8_t>(std::stringstream& ss, uint8_t& head) {
        ss << std::to_string((int) head);
    }

    template<>
    void FleXdLog<bool>(std::stringstream& ss, bool& head) {
        if (head) {
            ss << "true";
        } else {
            ss << "false";
        }
    }
}

namespace flexd {
    namespace logger {

        namespace LogLevel {
            enum Enum {
                VERBOSE = 0x00,
                DEBUG,
                INFO,
                WARN,
                ERROR,
                FATAL
                // TODO other types of message
            };
        }

        class FleXdLogger {

        public:
            ~FleXdLogger();
            static FleXdLogger& instance();
            template<typename... T>
            void log(LogLevel::Enum logLevel, const std::string& level, std::thread::id threadId, std::time_t time, T... logs)
            {
                std::stringstream ss;
                FleXdLog(ss, logs...);
                writeLog(logLevel, time, level, ss.str());
            }
            bool loggerInit(flexd::icl::ipc::FleXdEpoll& poller, const std::string& appID);
            FleXdLogger(FleXdLogger const&) = delete;
            void operator=(FleXdLogger const&) = delete;

        private:
            FleXdLogger();
            void initLogger();
            void writeLog(LogLevel::Enum logLevel, std::time_t time, const std::string& level, const std::string& stream);
            void writeLogToBuffer(const LogLevel::Enum logLevel, const std::string& stream, std::time_t time);
            void handshake();

        private:
            bool m_connectionToServer;
            std::string m_appName;
            uint16_t m_appIDuint;
            LogLevel::Enum m_flexLogLevel;
            std::shared_ptr<FleXdLogBuffer> m_logBuffer;
            std::unique_ptr<FleXdLoggerIPCClient> m_IPCClient;

            uint8_t m_msgCount;
        };

    } // namespace logger
} // namespace flexd

#endif /* FLEXDLOGGER_H */
