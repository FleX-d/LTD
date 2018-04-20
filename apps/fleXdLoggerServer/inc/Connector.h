/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Connector.h
 * Author: dev
 *
 * Created on April 13, 2018, 1:03 PM
 */

#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "FleXdEpoll.h"
#include "FleXdUDSServer.h"

namespace flexd {
    namespace FlexLogger {

        class Connector : public flexd::ilc::epoll::FleXdUDSServer 
        {
        public:
            Connector();
            virtual ~Connector();

            virtual void onWrite(flexd::ilc::epoll::pSharedFleXdIPCMsg msg);

            virtual void onMessage(flexd::ilc::epoll::pSharedFleXdIPCMsg msg);


            Connector(const Connector& orig) = delete;
            Connector& operator=(const Connector&) = delete;

        private:
            flexd::ilc::epoll::FleXdEpoll m_pooler(10);

        };

    } // namespace FlexLogger
} // namespace flexd       


#endif /* CONNECTOR_H */

