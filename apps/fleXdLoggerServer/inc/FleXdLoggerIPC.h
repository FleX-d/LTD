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

//TODO: remove circular dependency
//#include <FleXdEpoll.h>
//#include <FleXdUDSServer.h>
#include "FleXdLogMessage.h"

namespace flexd {
    namespace logger {

        class FleXdLoggerIPC //: public flexd::icl::epoll::FleXdUDSServer //TODO: remove circular dependency
        {
        public:
            FleXdLoggerIPC(std::function<void(FleXdLogMessage)> logging);
            virtual ~FleXdLoggerIPC();

//TODO: remove circular dependency
//            void onWrite(flexd::icl::epoll::pSharedFleXdIPCMsg msg);
//            void onMessage(flexd::icl::epoll::pSharedFleXdIPCMsg msg);

            

            FleXdLoggerIPC(const FleXdLoggerIPC& orig) = delete;
            FleXdLoggerIPC& operator=(const FleXdLoggerIPC&) = delete;

        private:
//TODO: remove circular dependency
//            flexd::icl::epoll::FleXdEpoll m_pooler;
            std::function<void(FleXdLogMessage)> m_logging;

        };

    } // namespace logger
} // namespace flexd       


#endif /* FLEXDCONNECTOR_H */

