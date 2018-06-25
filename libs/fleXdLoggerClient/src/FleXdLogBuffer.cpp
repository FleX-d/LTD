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
 * File:   FleXdLogBuffer.cpp
 * Author: Jakub Pekar
 *
 * Created on May 3, 2018, 1:25 PM
 */

#include "FleXdLogBuffer.h"

namespace flexd {
    namespace logger {

        FleXdLogBuffer::FleXdLogBuffer(size_t maxSizeBuffer)
        : m_maxSizeBuffer(maxSizeBuffer),
          m_sizeBuffer(0),
          m_queue(),
          m_mutex() {
        }

        uint32_t FleXdLogBuffer::getSizeBuffer() {
            if (m_queue.empty()) {
                m_sizeBuffer = 0;
            }
            return m_sizeBuffer;
        }

        const LogData& FleXdLogBuffer::front() const {
            return m_queue.front();
        }

        bool FleXdLogBuffer::pop() {
            std::lock_guard<std::mutex> bufferLock(m_mutex);
            if (m_queue.empty()) {
                m_sizeBuffer = 0;
            }
            if (m_sizeBuffer > 0) {
                m_sizeBuffer -= m_queue.front().m_message.size();
                m_queue.pop();
                return true;
            }
            return false;
        }

        bool FleXdLogBuffer::push(LogData&& logData) {
            std::lock_guard<std::mutex> bufferLock(m_mutex);
            m_sizeBuffer += logData.m_message.size();// TODO getMessageLength return only size of payload without the header
            if (m_sizeBuffer < m_maxSizeBuffer){
                m_queue.push(std::move(logData));
                return true;
            } else {
                m_sizeBuffer -= logData.m_message.size();
            }
            return false;
        }

        std::unique_lock<std::mutex> FleXdLogBuffer::getLock() {
            return std::unique_lock<std::mutex>(m_mutex, std::defer_lock);
        }

    } // namespace FlexLogger
} // namespace flexd
