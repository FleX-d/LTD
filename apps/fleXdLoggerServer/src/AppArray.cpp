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

namespace flexd {
    namespace FlexLogger {

        AppArray::AppArray() {
            m_IDCounter = 0;
        }

        uint16_t AppArray::insertToArray(std::string appName, int p_descriptor) {
            //TODO control of equivalents object in map or equivalent of key;
            bool unique = true;
            if (m_IDCounter < m_IDCounter + 1) {
                for (Application& current : m_list) {
                    if (current.getAppName() == appName) {
                        unique = false;
                    }
                }

                if (unique) {
                    m_IDCounter++;
                    m_list.push_back(Application(appName, m_IDCounter, p_descriptor));
                } else {
                return 0;
            }
                return m_IDCounter;
            } else {
                //TODO overflow
                return 0;
            }
        }

        bool AppArray::removeFromArray(int p_descriptor) {
            for (Application& current : m_list) {
                if (current.getAppDescriptor() == p_descriptor) {
                    m_list.remove(current);
                    return true;
                }
            }
            return false;
        }

        AppArray::~AppArray() {
        }
    } // namespace FlexLogger
} // namespace flexd

