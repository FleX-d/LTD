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
 * File:   Logger.cpp
 * Author: Jakub Pekar
 */

#include <vector>

#include "Logger.h"
#include "Application.h"

namespace flexd {
    namespace FlexLogger {

        Logger::Logger() {
            // logLevelMax = MsgType::VERBOSE;
            int port = 15000;
            m_socServer = new iSocServer();
            m_socServer->connectFunck(port);
            m_socServer->listenServer();
        }

        bool Logger::loggingFunc() {
            int client, valread;
            std::string appName;
            char buffer[1024];
            std::vector<uint8_t> dataBuffer();
            uint16_t appId;
            
            openlog("LOGGER", LOG_CONS, LOG_LOCAL0);
            while (1) {
                //Connecting of client app        
                client = m_socServer->connectClient();
                std::cout << "Client connected " << std::endl;

                /*Handshake and generate of appID*/
                valread = m_socServer->recv(client, buffer, 1024);
                std::vector<uint8_t> dataBuffer(buffer, buffer + valread);
                if (valread) {
                    LogMessage initmessage(std::move(dataBuffer)); //create log init message
                    //appName = initmessage.getLogMessage();
                    appId = m_arrayOfApp.insertToArray(appName, client);

                    if (appId != 0) {
                        //std::cout << "*InitMessage* Appname: " << m_arrayOfApp.getApp(appId).getAppName() << std::endl;
                        LogMessage ackMessage(appId, (uint8_t) MsgType::SYSMSG_OK_NAME, initmessage.getMsgCounter() + 1, std::to_string(appId));
                        std::cout << "*ACKMESSAGE*-> ";
                        ackMessage.logToCout();
                        std::vector<uint8_t> streamVector = ackMessage.releaseData();
                        m_socServer->send(client, streamVector.data(), streamVector.size());
                    } else {
                        std::cout << "************This application name using other client*************" << std::endl;
                        LogMessage ackMessage(appId, (uint8_t) MsgType::SYSMSG_FALSE_NAME, initmessage.getMsgCounter() + 1, std::string("Error"));
                        std::cout << "*ACKMESSAGE*-> ";
                        ackMessage.logToCout();
                        std::vector<uint8_t> streamVector = ackMessage.releaseData();
                        m_socServer->send(client, streamVector.data(), streamVector.size());
                    }

                    /*Send appID*/


                }

                while (1) {
                    valread = m_socServer->recv(client, buffer, 1024);

                    dataBuffer = std::vector<uint8_t> (buffer, buffer + valread);
                    if (valread) {
                        LogMessage message(std::move(dataBuffer));

                        message.logToCout();
                        //message.logToSysLog(m_arrayOfApp.getApp(message.getAppID()).getAppName());
                        //logToSysLog(message);


                    } else {
                        //TODO remove application from map
                        m_arrayOfApp.removeFromArray(client);
                        close(client);

                        break;
                    }
                }

                
            }
            closelog();
        }/*
bool Logger::logToSysLog(LogMessage& message) {
    std::string priority;
    uint8_t sysLogType = 4;
    switch(message.getMsgType()){
        case 0:priority = "HANDSHAKE";
            //sysLogType = 6;
            break;
        case 1:priority = "VERBOSE";
            //sysLogType = 6;
            break;
        case 2:priority = "DEBUG";
           // sysLogType = 7;
            break;
        case 3:priority = "INFO";
            //sysLogType = 6;
            break;
        case 4:priority = "WARN";
            //sysLogType = 5;
            break;
        case 5:priority = "ERROR";
            //sysLogType = 3;
            break;
        case 6:priority = "FATAL";
            //sysLogType = 2;
            break;
        case 7:priority = "ALL";
            //sysLogType = 1;
            break;    
    }
    syslog(sysLogType ,"[%s][%ld][%s] %s ",m_arrayOfApp.getApp(message.getAppID()).getAppName, \
            message.getTime(),\
            priority.c_str(),
            message.getLogMessage().c_str()
            );
}*/

        Logger::~Logger() {
            delete m_socServer;
        }
    } // namespace FlexLogger
} // namespace flexd
