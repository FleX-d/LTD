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
 * File:   FleXdAppArray.h
 * Author: Jakub Pekar
 */

#ifndef FLEXDAPPARRAY_H
#define FLEXDAPPARRAY_H

#include <cstdint>
#include <vector>
#include <memory>
#include "FleXdApplication.h"


namespace flexd {
    namespace logger {

        class FleXdAppArray {
            typedef std::array<std::shared_ptr<FleXdApplication>, UINT16_MAX> appVec;
        public:
            FleXdAppArray();
            virtual ~FleXdAppArray();
            
            int insertToArray(const std::string& appName, int descriptor);              
            bool removeFromArray(int descriptor);
            bool unconnectApplication(int desctiptor);
            std::string getAppName(uint16_t appID) const;
            const std::shared_ptr<FleXdApplication> getApp(const std::string& appName)const;
            
            FleXdAppArray(const FleXdAppArray& orig) = delete;
        private:
            appVec m_list;
            appVec::iterator m_pos;
            uint16_t m_countOfConnectedApp;

        };

    } // namespace logger
} // namespace flexd

#endif /* FLEXDAPPARRAY_H */

