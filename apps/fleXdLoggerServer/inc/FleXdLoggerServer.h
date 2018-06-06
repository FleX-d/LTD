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
 * File:   FleXdLoggerServer.h
 * Author: Jakub Pekar
 */

#ifndef FLEXLOGGERSERVER_H
#define FLEXLOGGERSERVER_H


#include "FleXdLogStream.h"
#include "FleXdMessageType.h"
#include "FleXdAppArray.h"
#include "FleXdLoggerIPCServer.h"
#include "FleXdLoggerDlt.h"
#include "FleXdEpoll.h"
#include <sstream>
#include <thread>
#include <syslog.h>
#include <string>

namespace flexd {
    namespace logger {

        class FleXdLoggerServer {
        public:
            explicit FleXdLoggerServer(flexd::icl::ipc::FleXdEpoll& poller, bool logToDlt = false);
            virtual ~FleXdLoggerServer();
             /**
            * Function set on the client side the logLevel for filtering of logs.
            * @param appName - application identifier for which the log level should be changed
            * @param logLevel - log level value
            * @return true if set is successful, false otherwise
            */
            bool setLogLevel(const std::string& appName, MsgType::Enum logLevel);
             /**
            * Function await the client connection, than accept name of client and insert it to the AppArray, if its name is unique.
            * @return file descriptor of connected client, when occurred problem return negative value
            */
            //int handshake();
            int handshake(FleXdLogStream& hsMsg);

            FleXdLoggerServer(const FleXdLoggerServer&) = delete;
            FleXdLoggerServer& operator=(const FleXdLoggerServer&) = delete;
            FleXdLoggerServer(FleXdLoggerServer&&) = delete;
            FleXdLoggerServer& operator=(FleXdLoggerServer&&) = delete;

        private:
            bool logToSysLog(FleXdLogStream& message);
            void writeLog(const std::string appName,const uint64_t time, const std::string priority, const std::string message);

        private:
            std::shared_ptr<FleXdAppArray> m_arrayOfApp;
            FleXdLoggerDlt* m_fleXdLoggerDlt;
            FleXdLoggerIPC m_loggerIPCServer;
        };

    } // namespace logger
} // namespace flexd

#endif /* FLEXLOGGERSERVER_H */

