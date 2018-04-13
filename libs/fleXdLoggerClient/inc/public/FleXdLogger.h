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
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BlUT NOT LIMITED TO, THE IMPLIED
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
 * Author: Jakub Pekar, Branislav Podkonicky
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <algorithm>
#include <thread>
#include <iostream>
#include <sstream>


#define FLEX_LOG_TRACE(...)  \
    flexd::FlexLogger::Logger::instance().log(flexd::FlexLogger::MsgType::VERBOSE, "trace", std::this_thread::get_id(), std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count(), __VA_ARGS__)
#define FLEX_LOG_DEBUG(...)  \
    flexd::FlexLogger::Logger::instance().log(flexd::FlexLogger::MsgType::DEBUG, "debug", std::this_thread::get_id(), std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count(), __VA_ARGS__)
#define FLEX_LOG_INFO(...)  \
    flexd::FlexLogger::Logger::instance().log(flexd::FlexLogger::MsgType::INFO, "info", std::this_thread::get_id(), std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count(), __VA_ARGS__)
#define FLEX_LOG_WARN(...)  \
    flexd::FlexLogger::Logger::instance().log(flexd::FlexLogger::MsgType::WARN, "warning", std::this_thread::get_id(), std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count(), __VA_ARGS__)
#define FLEX_LOG_ERROR(...)  \
    flexd::FlexLogger::Logger::instance().log(flexd::FlexLogger::MsgType::ERROR, "error", std::this_thread::get_id(), std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count(), __VA_ARGS__)
#define FLEX_LOG_FATAL(...)  \
    flexd::FlexLogger::Logger::instance().log(flexd::FlexLogger::MsgType::FATAL, "fatal", std::this_thread::get_id(), std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count(), __VA_ARGS__)
#define FLEX_LOG_INIT(...)  \
    flexd::FlexLogger::Logger::instance().setAppId(__VA_ARGS__);
namespace {

    template<typename T_head>
    void FlexLog(std::stringstream& ss, T_head& head) {
        ss << head;
    }

    template<typename T_head, typename... T_tail>
    void FlexLog(std::stringstream& ss, T_head& head, T_tail&... tail) {
        FlexLog(ss, head);
        FlexLog(ss, tail...);
    }

    template<>
    void FlexLog<uint8_t>(std::stringstream& ss, uint8_t& head) {
        ss << std::to_string((int) head);
    }

    template<>
    void FlexLog<bool>(std::stringstream& ss, bool& head) {
        if (head) {
            ss << "true";
        } else {
            ss << "false";
        }
    }
}

namespace flexd {
    namespace FlexLogger {

        enum MsgType {
            HANDSHAKE = 0,
            VERBOSE = 1,
            DEBUG = 2,
            INFO = 3,
            WARN = 4,
            ERROR = 5,
            FATAL = 6,
            ALL = 7
            // TODO other types of message
        };

        class Logger {
        public:

            Logger() {
            }

            static Logger& instance() {
                static Logger INSTANCE;
                return INSTANCE;
            }
            ~Logger() = default;
            Logger(Logger const&) = delete;
            void operator=(Logger const&) = delete;

            template<typename... T>
            void log(MsgType logLevel, std::string level, std::thread::id threadId, std::time_t time, T... logs) {
                std::stringstream ss;
                FlexLog(ss, logs...);
                std::cout << "Logger: appID: " << 0 << " time: " << time << " LogLevel: " << level << " message: " << ss.str() << std::endl;
            }

           void setAppId(const std::string& id) {}

        };
    } // namespace FlexLogger
} // namespace flexd


#endif /* LOGGER_H */

