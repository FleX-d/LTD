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

#include "Connector.h"

namespace flexd {
    namespace FlexLogger {

        Connector::Connector() 
        : flexd::ilc::epoll::FleXdUDSServer("/tmp/fleXdLogger.soc",m_pooler)
        {
            this->initialization();
            m_pooler.loop();
        }


        Connector::~Connector() {
        }

    } // namespace FlexLogger
} // namespace flexd    
