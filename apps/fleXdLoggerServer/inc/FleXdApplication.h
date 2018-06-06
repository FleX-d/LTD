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
 * File:   FleXdApplication.h
 * Author: Jakub Pekar
 */

#ifndef FLEXDAPPLICATION_H
#define FLEXDAPPLICATION_H

#include "FleXdMessageType.h"
#include <cstring>
#include <iostream>
#include <stdint.h>

namespace flexd {
    namespace logger {

        class FleXdApplication {

        public:
            FleXdApplication(const std::string& appName, int appDescriptor); // Set default loglevel
            FleXdApplication(const FleXdApplication& orig);
            virtual ~FleXdApplication();
             /**
            * Function set the log level for log filtering
            * @param logLevel - value for log filtering
            */
            void setLogLevel(MsgType::Enum logLevel);
             /**
            * Function set the state of application to online, if is connected
            */
            void setOnline();
             /**
            * Function set the state of application to offline, if is unconnected
            */
            void setOffline();
             /**
            * Function set the socket descriptor of application after reconnection
            * @param appFileDescriptor - new socket descriptor
            */
            void setAppDescriptor(int appFileDescriptor);
             /**
            * Function gets the name of the application.
            * @return the name of application
            */
            std::string getAppName()const;
            /**
            * Function gets the assigned socket descriptor of the application.
            * @return the socket descriptor of application
            */
            int getAppDescriptor();
            /**
            * Function gets the set log level .
            * @return the log level for filtering logs
            */
            MsgType::Enum getLogLevel();
            /**
            * Function controls the value of attribute application 
            * @return true if the m_online is set to true and application is running,false otherwise
            */
            bool isOnline() const;
            /**
            * Function compares the name of application with parameter
            * @param name - comparing string that input to the function
            * @return true if the name of the application is compare with parameter, false otherwise 
            */
            bool compareName(const std::string& name);
            
        private:
            std::string m_appName;
            int m_appFileDesc;
            bool m_online; // represent if app is active or not
            MsgType::Enum m_logLevel;
        };
        
    } // namespace logger
} // namespace flexd


#endif /* FLEXDAPPLICATION_H */

