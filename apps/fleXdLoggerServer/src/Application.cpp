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
 * File:   Application.cpp
 * Author: Jakub Pekar
 */



#include <FleXdLogger.h>

#include "Application.h"
namespace flexd {
    namespace FlexLogger {

        Application::Application(const std::string& appName, int appDescriptor)
        : m_appName(appName),      
          m_appFileDesc(appDescriptor),
          m_online(true),
          m_logLevel(MsgType::Enum::VERBOSE){
        }
        void Application::setOnline() {
            m_online = true;
        }

        void Application::setOffline() {
            m_online = false;
        }
        void Application::setAppDescriptor(int appFileDescriptor) {
            m_appFileDesc = appFileDescriptor;
        }
        void Application::setLogLevel(MsgType::Enum logLevel) {
            m_logLevel = logLevel;
        }


        MsgType::Enum Application::getLogLevel(){
            return m_logLevel;
        }

        bool Application::isOnline() const {
            return m_online;
        }
        
        std::string Application::getAppName() const{
            return m_appName;
        }

        int Application::getAppDescriptor(){
            return m_appFileDesc;
        }

        bool Application::compareName(const std::string& name) {
            if(m_appName == name){
                return true;
            } else {
                return false;
            }
        }

        Application::Application(const Application& orig) {
            this->m_appFileDesc =orig.m_appFileDesc;
            this->m_appName = orig.m_appName;
        }


        Application::~Application() {
        }
    } // namespace FlexLogger
} // namespace flexd

