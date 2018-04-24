/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FleXdLoggerIPC.cpp
 * Author: dev
 * 
 * Created on April 13, 2018, 1:03 PM
 */


#include "FleXdLoggerIPC.h"
#include "FleXdLogMessage.h"
#include "FleXdLoggerServer.h"

namespace flexd {
    namespace logger {

        FleXdLoggerIPC::FleXdLoggerIPC(std::function<void(FleXdLogMessage)> logging) 
        :m_pooler(10), 
        flexd::icl::epoll::FleXdUDSServer("/tmp/fleXdLogger.soc",m_pooler)          
        {
            
            this->initialization();
            m_pooler.loop();
        }
        
        void FleXdLoggerIPC::onMessage(flexd::icl::epoll::pSharedFleXdIPCMsg msg) {
            if(msg)
            {
                std::vector<uint8_t> recvPayload;
                recvPayload = msg->getPayload();
                FleXdLogMessage recvLog(std::move(recvPayload));   
                
            } else {
                // fail message
                
            }
        }

        FleXdLoggerIPC::~FleXdLoggerIPC() {
        }

    } // namespace logger
} // namespace flexd    
