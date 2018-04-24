/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FleXdConnector.h
 * Author: dev
 *
 * Created on April 13, 2018, 1:03 PM
 */

#ifndef FLEXDLOGGERIPC_H
#define FLEXDLOGGERIPC_H

#include "FleXdEpoll.h"
#include "FleXdUDSServer.h"
#include "FleXdLogMessage.h"

namespace flexd {
    namespace logger {

        class FleXdLoggerIPC : public flexd::icl::epoll::FleXdUDSServer 
        {
        public:
            FleXdLoggerIPC(std::function<void(FleXdLogMessage)> logging);
            virtual ~FleXdLoggerIPC();

            void onWrite(flexd::icl::epoll::pSharedFleXdIPCMsg msg);
            void onMessage(flexd::icl::epoll::pSharedFleXdIPCMsg msg);

            

            FleXdLoggerIPC(const FleXdLoggerIPC& orig) = delete;
            FleXdLoggerIPC& operator=(const FleXdLoggerIPC&) = delete;

        private:
            flexd::icl::epoll::FleXdEpoll m_pooler;
            std::function<void(FleXdLogMessage)> m_logging;

        };

    } // namespace logger
} // namespace flexd       


#endif /* FLEXDCONNECTOR_H */

