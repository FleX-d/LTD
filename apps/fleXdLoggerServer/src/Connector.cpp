/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Connector.cpp
 * Author: dev
 * 
 * Created on April 13, 2018, 1:03 PM
 */

#include <FleXdUDS.h>

#include "Connector.h"
#include "LogMessage.h"

namespace flexd {
    namespace FlexLogger {

        Connector::Connector() 
        :m_pooler(10), 
        flexd::ilc::epoll::FleXdUDSServer("/tmp/fleXdLogger.soc",m_pooler)
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

    } // namespace FlexLogger
} // namespace flexd    
