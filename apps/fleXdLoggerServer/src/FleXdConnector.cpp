/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FleXdConnector.cpp
 * Author: dev
 * 
 * Created on April 13, 2018, 1:03 PM
 */

#include <FleXdUDS.h>

#include "FleXdConnector.h"
#include "FleXdLogMessage.h"

namespace flexd {
    namespace logger {

        Connector::Connector() 
        : flexd::ilc::epoll::FleXdUDSServer("/tmp/fleXdLogger.soc",m_pooler),
          m_pooler(10)
        {
            this->initialization();
            m_pooler.loop();
        }
        
        void Connector::onMessage(flexd::ilc::epoll::pSharedFleXdIPCMsg msg) {
            if(msg)
            {
                std::vector<uint8_t> recvPayload;
                recvPayload = msg->getPayload();
                LogMessage recvLog();
                
                
            } else {
                // fail message
                
            }
        }



        Connector::~Connector() {
        }

    } // namespace logger
} // namespace flexd    
