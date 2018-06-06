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
 * File:   FleXdLoggerIPCClient.h
 * Author: dev
 *
 * Created on May 23, 2018, 3:54 PM
 */

#ifndef FLEXDLOGGERIPCCLIENT_H
#define FLEXDLOGGERIPCCLIENT_H

#include <string>
#include "FleXdEpoll.h"
#include "FleXdIPCProxyBuilder.h"
#include "FleXdIPCMsg.h"
#include "FleXdUDSClient.h"
#include "FleXdLogBuffer.h"

namespace flexd {
    namespace logger {

        class FleXdLoggerIPCClient : public flexd::icl::ipc::FleXdIPCProxyBuilder<flexd::icl::ipc::FleXdUDSClient> {
        public:
            FleXdLoggerIPCClient(std::shared_ptr<FleXdLogBuffer> logBuffer, flexd::icl::ipc::FleXdEpoll& poller);
            virtual ~FleXdLoggerIPCClient() = default;

            virtual void rcvMsg(flexd::icl::ipc::pSharedFleXdIPCMsg msg, int fd = 0) override;
            void onConnect(bool ret);
            void onDisconnect(bool ret);

            void setName(std::string name);
            std::string getName()const;
            uint16_t getAppID()const;
            bool isConnected();

            void flushBuffer();

            FleXdLoggerIPCClient(const FleXdLoggerIPCClient&) = delete;

        private:
            bool handshake();
            
        private:
            bool m_connectionToServer;
            std::string m_appName;
            uint16_t m_appIDuint;
            std::shared_ptr<FleXdLogBuffer> m_logBuffer;
        };

    } // namespace logger
} // namespace flexd
#endif /* FLEXDLOGGERIPCCLIENT_H */

