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
 * File:   FleXdLoggerServer.cpp
 * Author: Jakub Pekar
 */

#include "FleXdLoggerServer.h"
#include "FleXdApplication.h"
#include "FleXdIPCMsg.h"
#include <vector>

namespace flexd {
    namespace logger {

        FleXdLoggerServer::FleXdLoggerServer(flexd::icl::ipc::FleXdEpoll& poller, bool logToDlt /*= false*/)
        : m_arrayOfApp(new FleXdAppArray()),
          m_loggerIPCServer(m_arrayOfApp, poller, [&](FleXdLogStream& message)->bool{ return logToSysLog(message); }, logToDlt)
        {
            openlog("FLEXDLOGGER", LOG_CONS, LOG_LOCAL0);
            writeLog("FleXdLoggerServer",0,"INFO"," -> Init");
            m_loggerIPCServer.init();
        }

        bool FleXdLoggerServer::logToSysLog(FleXdLogStream& message) {
            std::string priority;

            switch (message.getMsgType()) {
                case MsgType::Enum::VERBOSE:
                    priority = "VERBOSE";
                    break;
                case MsgType::Enum::DEBUG:
                    priority = "DEBUG";
                    break;
                case MsgType::Enum::INFO:
                    priority = "INFO";
                    break;
                case MsgType::Enum::WARN:
                    priority = "WARN";
                    break;
                case MsgType::Enum::ERROR:
                    priority = "ERROR";
                    break;
                case MsgType::Enum::FATAL:
                    priority = "FATAL";
                    break;
		case MsgType::Enum::HANDSHAKESUCCES:
		    priority = "HANDSHAKESUCCES";
		    break;
		case MsgType::Enum::HANDSHAKEFAIL:
		    priority = "HANDSHAKEFAIL";
		    break;
                default:
                    priority = "SYSTEM/UNKNOW";
                    break;
            }
            if(message.getAppID() == 0){
	      writeLog("FleXdLoggerServer", message.getTime(), priority, message.getMessage());
	    } else {
	      writeLog(m_arrayOfApp->getAppName(message.getAppID()), message.getTime(), priority, message.getMessage());
	    }
            return true;
        }

        void FleXdLoggerServer::writeLog(const std::string appName,const uint64_t time, const std::string priority, const std::string message) {
            uint8_t sysLogType = 4;
            uint64_t timeVal;
            if(time == 0)
            {
                timeVal = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            } else {
                timeVal = time;
            }

            std::cout << "[" << appName << "]" << "[" << std::to_string(timeVal) << "]" << "[" << priority << "] : " << message << std::endl;
            syslog(sysLogType,"[%s][%lld][%s] %s ",
                    appName.c_str(), timeVal, priority.c_str(), message.c_str());
        }

        bool FleXdLoggerServer::setLogLevel(const std::string& appName, MsgType::Enum logLevel) {
            // TODO browse map and set log level for required app
            // check if app is active and if yes send sysMsg for change loglevel
            // don't forget write process log to syslog
            std::shared_ptr<FleXdApplication> app = m_arrayOfApp->getApp(appName);
            app->setLogLevel(logLevel);

            //TODO check the return value
            m_loggerIPCServer.setLogLevel(appName, logLevel);

            if(app->isOnline())
            {/*
                FleXdLogMessage msgSendLog(0,MsgType::Enum::SETLOGLEVEL,0,std::string(1,(uint8_t)logLevel));
                std::vector<uint8_t> streamVector = msgSendLog.releaseData();
                if(valread > 0)
                {
                   FleXdLogMessage ackSetLogLvl(std::move(dataBuffer));
                   if(ackSetLogLvl.getMsgType() == MsgType::Enum::SETLOGLEVELACKSUCCES){
                       writeLog("FleXdLoggerServer",0,"INFO","Set the log level on: " + appName + ". Application is online");
                       return true;
                   }
                }
                writeLog("FleXdLoggerServer",0,"WARN"," -> Can`t set log level on: " + appName );
                return false;
                */
            }

            writeLog("FleXdLoggerServer", 0, "INFO", " -> Set the log level on: " + appName + ". Application is offline");
            return true;

        }

        FleXdLoggerServer::~FleXdLoggerServer() {
            closelog();
        }

    } // namespace logger
} // namespace flexd
