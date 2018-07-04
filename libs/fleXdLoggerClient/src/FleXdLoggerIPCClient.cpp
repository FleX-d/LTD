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
 * File:   FleXdLoggerIPCClient.cpp
 * Author: Jakub Pekar
 *
 * Created on May 23, 2018, 3:54 PM
 */

#include <iostream>
#include "FleXdLoggerIPCClient.h"

using namespace flexd::icl::ipc;
namespace flexd {
    namespace logger {

        FleXdLoggerIPCClient::FleXdLoggerIPCClient(std::shared_ptr<FleXdLogBuffer> logBuffer, flexd::icl::ipc::FleXdEpoll& poller)
        : FleXdIPCProxyBuilder<FleXdUDSClient>("/tmp/FleXd/shared/ipc/uds/fleXdLogger.soc", poller),
          m_connectionToServer(false),
          m_appName(""),
          m_appIDuint(0),
          m_flexLogLevel(MsgType::Enum::VERBOSE),
          m_logBuffer(logBuffer) {
            this->setOnConnect([this](bool ret){ this->onConnect(ret); });
            this->setOnDisconnect([this](int ret){ this->onDisconnect(ret); });
        }

        void FleXdLoggerIPCClient::setName(std::string name) {
            m_appName = name;
        }

        uint16_t FleXdLoggerIPCClient::getAppID() const {
            return this->m_appIDuint;
        }

        std::string FleXdLoggerIPCClient::getName() const {
            return  m_appName;
        }

        MsgType::Enum FleXdLoggerIPCClient::getLogLvlFilter(){
            return m_flexLogLevel;
        }

        bool FleXdLoggerIPCClient::isConnected() {
            return m_connectionToServer;
        }

        void FleXdLoggerIPCClient::flushBuffer() {
            if (m_appIDuint != 0 && m_connectionToServer && m_logBuffer) {
                auto guard = m_logBuffer->getLock();
                while (true) {
                    guard.lock();
                    if(m_logBuffer->getSizeBuffer()) {
                        const LogData& data(m_logBuffer->front());
                        FleXdLogStream sendStream(m_appIDuint, data.m_messageType, data.m_messageCounter, data.m_message, data.m_time);
                        sndMsg(std::make_shared<flexd::icl::ipc::FleXdIPCMsg>(data.m_messageType, sendStream.releaseData()));
                        guard.unlock();
                        m_logBuffer->pop();
                    } else {
                        guard.unlock();
                        break;
                    }
                }
            }
            // mutex is unlocked when guard goes out of scope
        }

        void FleXdLoggerIPCClient::rcvMsg(pSharedFleXdIPCMsg msg, int fd) {
            if (msg) {
                if (!msg->getHeaderParamType()) {
                    uint8_t msgType = msg->getHeaderParam();
                    std::vector<uint8_t> recvVector;
                    FleXdLogStream recvMsg(msg->releasePayload());
                    switch (msgType) {
                        case MsgType::Enum::HANDSHAKESUCCES:
                            m_connectionToServer = true;
                            m_appIDuint = recvMsg.getAppID();
                            flushBuffer();
                            break;
                        case MsgType::Enum::HANDSHAKEFAIL:
                            std::cout << "FleXdLogger::[" << m_appName << "][" << m_appIDuint << "][HandshakeFail] : " << "Handshake failure. This Name is using" << std::endl;
                            m_connectionToServer = false;
                            break;
                        case MsgType::SETLOGLEVEL://TODO  setting of loglevel
                            ;
                            break;
                        default:
                            ;
                            break;
                    }
                }
            }
        }

        void FleXdLoggerIPCClient::onConnect(bool ret) {
            //TODO
        }

        void FleXdLoggerIPCClient::onDisconnect(int ret) {
            m_connectionToServer = false;
        }

    } // namespace logger
} // namespace flexd


