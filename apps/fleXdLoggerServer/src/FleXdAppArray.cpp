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
 * File:   FleXdAppArray.cpp
 * Author: Jakub Pekar
 */

#include "FleXdAppArray.h"

#include <limits>

namespace flexd {
    namespace logger {

        FleXdAppArray::FleXdAppArray()
        : m_pos(m_list.begin()),
        m_rangeOverFlow(false)
        {
            std::fill(m_list.begin(),m_list.end(),nullptr);
        }
        /* Function for inserting application descriptor into vector.
         * The function checks the unique of the application name and return the unique ID
         * from range  0 - 65535.
         */
        int FleXdAppArray::insertToArray(const std::string& appName, int descriptor) {
            uint16_t countOfControledPtr = 0;
            for (auto ref = m_list.begin(); ref < m_list.end(); ref++) {
                if (ref->get()) {
                    if(++countOfControledPtr > m_countOfCreatedApp){          // If loop checked all pointers in list,break loop. 
                        break;
                    }
                    if (ref->get()->compareName(appName)) {
                        if(!ref->get()->isOnline()){
                            ref->get()->setAppDescriptor(descriptor);
                            ref->get()->setOnline();
                            return (ref - m_list.begin() );
                        } else {
                            return -1;
                        }
                    }
                }
            }

            auto addApp = [&appName, &descriptor](appVec::iterator it) {
                auto tmp = std::make_shared<FleXdApplication>(appName,descriptor);
                it->swap(tmp);
            };

            if (++m_pos >= m_list.end()) {
               m_rangeOverFlow = true; 
            } 
            if(!m_rangeOverFlow){
               return insertToArray(addApp);
            } else {
              return  insertToOverFlowArray(addApp);
            }
            return -2;
        }
        
        int FleXdAppArray::insertToArray(std::function<void(appVec::iterator) > funcSwapingPointer) {
            if (m_pos->get() == nullptr) {
                funcSwapingPointer(m_pos);
                m_countOfCreatedApp++;
                return (m_pos - m_list.begin() );
            }
            return -2;
        }
        
        int FleXdAppArray::insertToOverFlowArray(std::function<void(appVec::iterator) > funcSwapingPointer) {
            for (auto pos = m_list.begin(); pos < m_list.end(); pos++) {
                if (pos->get()->isOnline()) {
                    m_pos = pos;
                    funcSwapingPointer(m_pos);
                    m_countOfCreatedApp++;
                    return (m_pos - m_list.begin());
                }
            }
            return -2;
        }


        std::string FleXdAppArray::getAppName(uint16_t appID) const {
            if (m_list[appID].get()) {
                return m_list[appID]->getAppName();
            }
            return std::string{};
        }
        
        const std::shared_ptr<FleXdApplication> FleXdAppArray::getApp(const std::string& appName) const {
            for (auto app : m_list) 
            {
                if(app->compareName(appName))
                {
                    return app;
                }
            }
            return nullptr;
        }
        
        bool FleXdAppArray::unconnectApplication(int desctiptor) {
            for (auto& ref : m_list) {
                if (ref.get()) {
                    if (ref->getAppDescriptor() == desctiptor) {
                        ref->setOffline();
                        m_countOfCreatedApp--;
                        return true;
                    }
                }
            }
            return false;
        }


        bool FleXdAppArray::removeFromArray(int appDescriptor) {
            for (auto ref : m_list) {
                if (ref.get()) {
                    if (ref->getAppDescriptor() == appDescriptor) {
                        ref.reset();
                        m_countOfCreatedApp--;
                        return true;
                    }
                }
            }
            return false;
        }
        
        

        FleXdAppArray::~FleXdAppArray() {
        }
        
    } // namespace logger
} // namespace flexd

