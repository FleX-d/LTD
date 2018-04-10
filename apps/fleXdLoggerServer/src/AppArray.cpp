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
 * File:   AppArray.cpp
 * Author: Jakub Pekar
 */

#include "AppArray.h"

#include <limits>

namespace flexd {
    namespace FlexLogger {

        AppArray::AppArray()
        : m_pos(m_list.begin()) {
            memset(m_list.data(), 0, m_list.size());
        }

        int AppArray::insertToArray(const std::string& appName, int descriptor) {
            for (auto& ref : m_list) {
                if (ref.get()) {
                    if (ref->compareName(appName)) {
                        return -1;
                    }
                }
            }

            auto addApp = [&appName, &descriptor](appVec::iterator it) {
                auto tmp =std::make_unique<Application>(appName,descriptor);
                it->swap(tmp);
            };

            if (++m_pos < m_list.end()) {
                if (m_pos->get() == nullptr) {
                    addApp(m_pos);
                    return (m_pos - m_list.begin() );
                }
            } else {
                for (auto pos = m_list.begin(); pos < m_list.end(); pos++) {
                    if (pos->get() == nullptr) {
                        m_pos = pos;
                        addApp(m_pos);
                        return (m_pos - m_list.begin());
                    }
                }
            }
        }

        std::string AppArray::getAppName(uint16_t appID) const {
            if (m_list[appID].get()) {
                return m_list[appID]->getAppName();
            }
            return std::string{};
        }

        bool AppArray::removeFromArray(int appDescriptor) {
            for (auto& ref : m_list) {
                if (ref.get()) {
                    if (ref->getAppDescriptor() == appDescriptor) {
                        ref.release();
                        return true;
                    }
                }
            }
            return false;
        }

        AppArray::~AppArray() {
        }
    } // namespace FlexLogger
} // namespace flexd

