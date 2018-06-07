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
 * File:   FleXdLoggerIPC.cpp
 * Author: Jakub Pekar
 */

#include "FleXdLoggerIPCServer.h"
#include "FleXdLoggerServer.h"
#include "FleXdMessageType.h"
#include <algorithm>    // std::find

using namespace flexd::icl::ipc;
namespace flexd {
    namespace logger {

        FleXdLoggerIPC::FleXdLoggerIPC(std::shared_ptr<FleXdAppArray> appArray, FleXdEpoll& poller, std::function<bool(FleXdLogStream&)> logging)
        : FleXdIPCProxyBuilder<FleXdUDSServer>("/tmp/FleXd/shared/ipc/uds/fleXdLogger.soc", poller),
          m_appArray(appArray),
          m_logging(logging)
        {
            this->setOnConnectClient([this](int fd){ this->onConnectClient(fd); });
            this->setOnDisconnectClient([this](int fd){ this->onDisconnectClient(fd); });
        }

        bool FleXdLoggerIPC::handshake(FleXdLogStream& hsMsg, int fd)
        {
            auto it = std::find(m_undefineFD.begin(), m_undefineFD.end(), fd);
            if (it != m_undefineFD.end()) {
                m_undefineFD.erase(it);
                int appId = m_appArray->insertToArray(hsMsg.getMessage(), fd);
                if (appId > 0) {
                    FleXdLogStream ackMessage((uint16_t) appId, (uint8_t) MsgType::Enum::HANDSHAKESUCCES, 0, std::to_string(appId));
                    /*std::cout << "*ACK-"<< hsMsg.getMessage() <<  "* -> ";
                    ackMessage.logToCout();*/
		    m_logging(ackMessage);
		    
                    auto sendingIPCLog = std::make_shared<FleXdIPCMsg>((uint8_t) MsgType::Enum::HANDSHAKESUCCES, std::move(ackMessage.releaseData()));
                    this->sndMsg(sendingIPCLog, fd);

                    //TODO register application for Dlt logging, check the return value
                    //if (m_fleXdLoggerDlt != nullptr) {
                    //    m_fleXdLoggerDlt->registerApp( ... );
                    //}
                    return true;
                } else if (appId == -1) {
		    std::string msg = std::string("This applications name *(") + hsMsg.getMessage() + std::string(")* using other client.");
                    FleXdLogStream ackMessage(0, (uint8_t) MsgType::Enum::HANDSHAKEFAIL, 0, msg);
                   /* std::cout << "*ACK-"<< hsMsg.getMessage() <<  "* -> ";
                    ackMessage.logToCout();*/
		    m_logging(ackMessage);
                    auto sendingIPCLog  = std::make_shared<FleXdIPCMsg>((uint8_t) MsgType::Enum::HANDSHAKEFAIL, std::move(ackMessage.releaseData()));
                    this->sndMsg(sendingIPCLog, fd);

                    return false;
                } else {  
		  //TODO send ACK if is impossible accept next client
                    return false;
                }
                
            } else {
                //TODO handshake already done or descriptor missing in m_undefineFD
                return false;
            }
        }

        void FleXdLoggerIPC::rcvMsg(pSharedFleXdIPCMsg msg, int fd) {
            if(msg)
            {
                if(!msg->getHeaderParamType()){
                    uint8_t msgType = msg->getHeaderParam();
                    FleXdLogStream log(std::move(msg->releasePayload()));
                    switch (msgType) {
                        case MsgType::Enum::HANDSHAKE:
                            handshake(log, fd);
                            break;
                        default:
                            m_logging(log);
                            break;
                    }
                } else {
                 //TODO fail message
                }
            }
        }

        void FleXdLoggerIPC::onConnectClient(int fd) {
            m_undefineFD.push_back(fd);
        }

        void FleXdLoggerIPC::onDisconnectClient(int fd) {
            m_appArray->disconnectApplication(fd);
        }

    } // namespace logger
} // namespace flexd
