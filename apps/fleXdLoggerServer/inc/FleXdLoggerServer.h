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


#include "iSocServer.h"
#include "FleXdLogMessage.h"
#include "FleXdMessageType.h"
#include "FleXdAppArray.h"
#include <sstream>
#include <thread>
#include <syslog.h>
#include <string>

namespace flexd {
    namespace logger {
        
        class FleXdLoggerServer {
        public:
            FleXdLoggerServer();
            virtual ~FleXdLoggerServer();
            
            bool loggingFunc(const int client);                 //logging only for one application (TODO: threads?? Or solution is IPC?)
            bool setLogLevel(const std::string& appName, MsgType::Enum logLevel); 
            int handshake();
            
            FleXdLoggerServer(const FleXdLoggerServer& orig) = delete;
        private:
            bool logToSysLog(LogMessage& message); 
            void writeLog(const std::string appName,const uint64_t time, const std::string priority, const std::string message);
        private:
            AppArray m_arrayOfApp;
            //TODO IPC (now it run on sockets)
            iSocServer* m_socServer;
        };
        
    } // namespace logger
} // namespace flexd       

#endif /* FLEXLOGGERSERVER_H */

