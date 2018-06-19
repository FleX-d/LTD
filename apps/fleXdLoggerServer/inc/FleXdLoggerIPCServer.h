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
 * File:   FleXdLoggerIPC.h
 * Author: Jakub Pekar
 */

#ifndef FLEXDLOGGERIPC_H
#define FLEXDLOGGERIPC_H

#include <memory>
#include "FleXdIPCProxyBuilder.h"
#include "FleXdEpoll.h"
#include "FleXdUDSServer.h"
#include "FleXdLogStream.h"
#include "FleXdAppArray.h"

#ifdef DLT_ENABLED
    #include "FleXdLoggerDlt.h"
#endif /*DLT_ENABLED*/

namespace flexd {
    namespace logger {

        class FleXdLoggerIPCServer : public flexd::icl::ipc::FleXdIPCProxyBuilder<flexd::icl::ipc::FleXdUDSServer>
        {
        public:
            FleXdLoggerIPCServer(std::shared_ptr<FleXdAppArray> appArray, flexd::icl::ipc::FleXdEpoll& poller, std::function<bool(FleXdLogStream&)> logging, bool logToDlt);
            virtual ~FleXdLoggerIPCServer() = default;

            virtual void rcvMsg(flexd::icl::ipc::pSharedFleXdIPCMsg msg, int fd) override;
            void onConnectClient(int fd);
            void onDisconnectClient(int fd);

            FleXdLoggerIPCServer(const FleXdLoggerIPCServer& orig) = delete;
            FleXdLoggerIPCServer& operator=(const FleXdLoggerIPCServer&) = delete;

            /**
            * Function set on the client side the logLevel for filtering of logs.
            * @param appName - application identifier for which the log level should be changed
            * @param logLevel - log level value
            * @return true if set is successful, false otherwise
            */
            bool setLogLevel(const std::string& appName, MsgType::Enum logLevel);

        private:
            bool handshake(FleXdLogStream& hsMsg, int fd);

        private:
            std::shared_ptr<FleXdAppArray> m_appArray;
            std::function<bool(FleXdLogStream&)> m_logging;
#ifdef DLT_ENABLED
            std::unique_ptr<FleXdLoggerDlt> m_fleXdLoggerDlt;
#endif /*DLT_ENABLED*/
            std::vector<int> m_undefineFD;
        };

    } // namespace logger
} // namespace flexd


#endif /* FLEXDCONNECTOR_H */

