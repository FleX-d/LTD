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
#include <vector>

namespace flexd {
    namespace logger {

        FleXdLoggerServer::FleXdLoggerServer(bool logToDlt /*= false*/)
        : m_socServer(new iSocServer()),
          m_fleXdLoggerDlt(nullptr) {
            int port = 15000;
            m_socServer->connectFunck(port);
            m_socServer->listenServer();
           
            
            openlog("FLEXLOGGER", LOG_CONS, LOG_LOCAL0);
            writeLog("FleXdLoggerServer",0,"INFO"," -> Init");
            
            // Enable logging to Dlt
            if (logToDlt) {
                m_fleXdLoggerDlt = new FleXdLoggerDlt();
            }
        }   
        
        int FleXdLoggerServer::handshake()
        {
            int client, valread;
            uint8_t buffer[1024];
           
            //Connecting of client app        
            client = m_socServer->connectClient();
            std::cout << "Client connected " << std::endl;
            writeLog("FleXdLoggerServer",0,"INFO"," -> Client connected");

            /*Handshake and generate of appID*/
            valread = m_socServer->receive(client, buffer, 1024);
            std::vector<uint8_t>dataBuffer(buffer, buffer + valread);
            if (valread) {
            FleXdLogMessage initmessage(std::move(dataBuffer)); //create log init message
            int appId = m_arrayOfApp.insertToArray(initmessage.getLogMessage(), client);

                if (appId > 0) {
                    FleXdLogMessage ackMessage((uint16_t)appId, (uint8_t) MsgType::Enum::HANDSHAKESUCCES, 0, std::to_string(appId));
                    std::cout << "*ACKMESSAGE* -> ";
                    ackMessage.logToCout();
                    writeLog("FleXdLoggerServer",0,"VERBOSE",std::string(" -> Ack successful. *")+ m_arrayOfApp.getAppName(appId) + std::string("* has appID ") + std::to_string(appId));
                
                    std::vector<uint8_t> streamVector = ackMessage.releaseData();
                    m_socServer->send(client, streamVector.data(), streamVector.size());
                    
                    //TODO register application for Dlt logging, check the return value
                    //if (m_fleXdLoggerDlt != nullptr) {
                    //    m_fleXdLoggerDlt->registerApp( ... );
                    //}

                    //TODO when it will be implement IPC or something other it will be return the appId
                    return client;
                } else if (appId == -1) {
                    FleXdLogMessage ackMessage(appId, (uint8_t) MsgType::Enum::HANDSHAKEFAIL, 0, std::string("***This application name using other client***"));
                    std::cout << "*ACKMESSAGE*-> ";
                    ackMessage.logToCout();
                    writeLog("FleXdLoggerServer",0,"VERBOSE",std::string(" -> Handshake problem. This name is using."));
                    std::vector<uint8_t> streamVector = ackMessage.releaseData();
                    m_socServer->send(client, streamVector.data(), streamVector.size());
                    return -1;
                } else {
                    return -2;
                }
                //TODO send ACK if is impossible accept next client
            }else {
                return -3;
            }
            
        }

        bool FleXdLoggerServer::loggingFunc(const int client) // this will be implement in onMessage function in UDSServer
        {
            int valread;
            std::string appName;
            char buffer[1024];

            while (true) 
            {
                valread = m_socServer->receive(client, buffer, 1024);
                std::vector<uint8_t> dataBuffer(buffer, buffer + valread);
                if (valread) 
                {
                    FleXdLogMessage message(std::move(dataBuffer));
                    message.logToCout();
                    logToSysLog(message);
                    //TODO send log to Dlt, check the return value
                    //if (m_fleXdLoggerDlt != nullptr) {
                    //    m_fleXdLoggerDlt->sendLog( ... , static_cast<MsgType::Enum>(message.getMsgType()), message.getLogMessage());
                    //}
                } else {
                    //TODO unregister app from Dlt logging, check the return value
                    //if (m_fleXdLoggerDlt != nullptr) {
                    //    m_fleXdLoggerDlt->unregisterApp( ... );
                    //}
                    std::cout << "Client is unconnected!"<< std::endl;
                    m_arrayOfApp.unconnectApplication(client);
                    close(client);
                    break;
                }
            } 
            return true;
        }
        // rename to logging, when it will be able to implement ICL
        bool FleXdLoggerServer::logToSysLog(FleXdLogMessage& message) {
            std::string priority;
            
            switch (message.getMsgType()) {
                case MsgType::Enum::HANDSHAKE: priority = "HANDSHAKE";
                    break;
                case MsgType::Enum::VERBOSE:priority = "VERBOSE";
                    break;
                case MsgType::Enum::DEBUG:priority = "DEBUG";
                    break;
                case MsgType::Enum::INFO:priority = "INFO";
                    break;
                case MsgType::Enum::WARN:priority = "WARN";
                    break;
                case MsgType::Enum::ERROR:priority = "ERROR";
                    break;
                case MsgType::Enum::FATAL:priority = "FATAL";
                    break;
                default:
                    priority = "SYSTEM/UNKNOW";
                    break;
            }
            writeLog(m_arrayOfApp.getAppName(message.getAppID()), message.getTime(), priority, message.getLogMessage());
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
            syslog(sysLogType,"[%s][%ld][%s] %s ",
                    appName.c_str(), timeVal, priority.c_str(), message.c_str());
        }

        
        bool FleXdLoggerServer::setLogLevel(const std::string& appName, MsgType::Enum logLevel) {
            // TODO browse map and set log level for required app
            // check if app is active and if yes send sysMsg for change loglevel
            // don't forget write process log to syslog
            std::shared_ptr<FleXdApplication> app = m_arrayOfApp.getApp(appName);
            app->setLogLevel(logLevel);

            //TODO set log level for application when Dlt logging is enabled, check the return value
            //if (m_fleXdLoggerDlt != nullptr) {
            //    m_fleXdLoggerDlt->setLogLevel( ... );
            //}
 
            if(app->isOnline())
            {
                FleXdLogMessage msgSendLog(0,MsgType::Enum::SETLOGLEVEL,0,std::string(1,(uint8_t)logLevel));
                std::vector<uint8_t> streamVector = msgSendLog.releaseData();
                m_socServer->send(app->getAppDescriptor(), streamVector.data(), streamVector.size());
                
                //Recv acknowledge about syslog setting 
                char buffer[128];
                int valread = m_socServer->receive(app->getAppDescriptor(),buffer, 128);
                std::vector<uint8_t>dataBuffer(buffer, buffer + valread);
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
                
            }
            
            writeLog("FleXdLoggerServer",0,"INFO"," -> Set the log level on: " + appName + ". Application is offline");
            return true;
            
        }
   
        FleXdLoggerServer::~FleXdLoggerServer() {
            delete m_socServer;
            if (m_fleXdLoggerDlt != nullptr) {
                delete m_fleXdLoggerDlt;
            }
            closelog();
        }
    } // namespace logger
} // namespace flexd