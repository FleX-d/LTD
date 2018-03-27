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
 * File:   Application.h
 * Author: Jakub Pekar
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <cstring>
#include <iostream>
#include <stdint.h>

namespace flexd {
    namespace FlexLogger {

        class Application {

        public:
            Application(std::string appName, uint16_t p_appID, int p_appDescriptor);
            Application(const Application& orig);
            virtual ~Application();
            uint16_t getAppID();
            std::string getAppName();
            int getAppDescriptor();
            bool compareName(std::string& p_name);
            bool operator == (const Application& lhs)const{ return this->m_appFileDesc == lhs.m_appFileDesc &&
                    this->m_appID == lhs.m_appID;}
            
        private:
            uint16_t m_appID;
            std::string m_appName;
            int m_appFileDesc;
        };
        
    } // namespace FlexLogger
} // namespace flexd


#endif /* APPLICATION_H */
