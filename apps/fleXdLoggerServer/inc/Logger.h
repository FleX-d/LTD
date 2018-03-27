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
 * File:   Logger.h
 * Author: Jakub Pekar
 */

/*
 * LOG_PID|LOG_CONS,
 * 
 * LOG_CONS       Write directly to system console if there is
                  an error while sending to system logger.

   LOG_NDELAY     Open the connection immediately (normally, the
                  connection is opened when the first message is
                  logged).

   LOG_NOWAIT     Don't wait for child processes that may have
                  been created while logging the message.  (The
                  GNU C library does not create a child process,
                  so this option has no effect on Linux.)

   LOG_ODELAY     The converse of LOG_NDELAY; opening of the
                  connection is delayed until syslog() is
                  called.  (This is the default, and need not be
                  specified.)

   LOG_PERROR     (Not in POSIX.1-2001.)  Print to stderr as
                  well.

   LOG_PID        Include PID with each message.
 * **************************************************************************
LOG_EMERG  0
    The message says the system is unusable. 
LOG_ALERT
    Action on the message must be taken immediately. 
LOG_CRIT
    The message states a critical condition. 
LOG_ERR
    The message describes an error. 
LOG_WARNING
    The message is a warning. 
LOG_NOTICE
    The message describes a normal but important event. 
LOG_INFO
    The message is purely informational. 
LOG_DEBUG  7
    The message is only for debugging purposes. 
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <sstream>
#include <thread>
#include <syslog.h>
#include "iSocServer.h"
#include "LogMessage.h"
#include "MessageType.h"
#include "AppArray.h"

namespace flexd {
    namespace FlexLogger {
        class Logger {
        public:
            Logger();
            Logger(const Logger& orig) = delete;
            virtual ~Logger();
            
            
            //bool logToSysLog(LogMessage& message); 
            bool loggingFunc();                 //logging only for one application (TODO: threads?? Or solution is IPC?)
        private:
            AppArray m_arrayOfApp;
            //TODO IPC (now it run on sockets)
            iSocServer* m_socServer;
            MsgType m_logLevelMax;
            
        };
        
    } // namespace FlexLogger
} // namespace flexd       
    
    






















/*
#ifndef FLEX_LOG_LEVEL
#define FLEX_LOG_LEVEL flexd::FlexLogger::LogLevelsEnum::ALL
#endif

#define FLEX_LOG_ALL(...)  \
    flexd::FlexLogger::Logger::instance().log(flexd::FlexLogger::LogLevelsEnum::ALL, "all", std::this_thread::get_id(), std::time(nullptr), __VA_ARGS__)
#define FLEX_LOG_TRACE(...)  \
    flexd::FlexLogger::Logger::instance().log(flexd::FlexLogger::LogLevelsEnum::TRACE, "trace", std::this_thread::get_id(), std::time(nullptr), __VA_ARGS__)
#define FLEX_LOG_DEBUG(...)  \
    flexd::FlexLogger::Logger::instance().log(flexd::FlexLogger::LogLevelsEnum::DEBUG, "debug", std::this_thread::get_id(), std::time(nullptr), __VA_ARGS__)
#define FLEX_LOG_INFO(...)  \
    flexd::FlexLogger::Logger::instance().log(flexd::FlexLogger::LogLevelsEnum::INFO, "info", std::this_thread::get_id(), std::time(nullptr), __VA_ARGS__)
#define FLEX_LOG_WARN(...)  \
    flexd::FlexLogger::Logger::instance().log(flexd::FlexLogger::LogLevelsEnum::WARN, "warning", std::this_thread::get_id(), std::time(nullptr), __VA_ARGS__)
#define FLEX_LOG_ERROR(...)  \
    flexd::FlexLogger::Logger::instance().log(flexd::FlexLogger::LogLevelsEnum::ERROR, "error", std::this_thread::get_id(), std::time(nullptr), __VA_ARGS__)
#define FLEX_LOG_FATAL(...)  \
    flexd::FlexLogger::Logger::instance().log(flexd::FlexLogger::LogLevelsEnum::FATAL, "fatal", std::this_thread::get_id(), std::time(nullptr), __VA_ARGS__)




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
        
        enum LogLevelsEnum {
            HANDSHAKE = 0,
            VERBOSE = 1,
            DEBUG = 2,
            INFO = 3,
            WARN = 4,
            ERROR = 5,
            FATAL = 6,
            ALL = 7
        };

        class Logger {
        private:

            Logger() {
                
            }

        public:
            static Logger& instance();
            ~Logger() = default;
            Logger(Logger const&) = delete;
            void operator=(Logger const&) = delete;

            template<typename... T>
            void log(flexd::FlexLogger::LogLevelsEnum logLevel, std::string level, std::thread::id threadId, std::time_t time, T... logs) {
                std::string logMessage;
                std::stringstream ss;
                ss << "[thread ID:" << threadId << "] ";
                FlexLog(ss, logs...);
            }

            void setLogLevel(flexd::FlexLogger::LogLevelsEnum newLogLevel) {
            }
            
            void setAppIdd(){
            }
        };
    } // namespace FlexLogger
} // namespace flexd
*/

#endif /* LOGGER_H */

