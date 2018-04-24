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

#ifndef FLEXDCONNECTOR_H
#define FLEXDCONNECTOR_H

#include "FleXdEpoll.h"
#include "FleXdUDSServer.h"

namespace flexd {
    namespace logger {

        class Connector : public flexd::ilc::epoll::FleXdUDSServer 
        {
        public:
            Connector();
            virtual ~Connector();

            void onWrite(flexd::ilc::epoll::pSharedFleXdIPCMsg msg);
            void onMessage(flexd::ilc::epoll::pSharedFleXdIPCMsg msg);

            

            Connector(const Connector& orig) = delete;
            Connector& operator=(const Connector&) = delete;

        private:
            flexd::ilc::epoll::FleXdEpoll m_pooler;

        };

    } // namespace logger
} // namespace flexd       


#endif /* FLEXDCONNECTOR_H */

