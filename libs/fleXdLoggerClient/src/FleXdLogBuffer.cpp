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
#include "iSocClient.h"

namespace flexd {
    namespace logger {

        FleXdLogBuffer::FleXdLogBuffer(size_t maxSizeBuffer)
        : m_maxSizeBuffer(maxSizeBuffer),
        m_sizeBuffer(0),      
        m_queue(),
        m_mutex()        
        {      
        }
        
        uint32_t FleXdLogBuffer::getSizeBuffer() {
            return m_sizeBuffer;
        }


        LogStream& FleXdLogBuffer::getStream() {
            if (m_sizeBuffer > 0){
               return m_queue.front();
            }
        }
        
        bool FleXdLogBuffer::pop() {
            std::lock_guard<std::mutex> bufferLock(m_mutex);
            if (m_sizeBuffer > 0){
                m_sizeBuffer -= m_queue.front().getMessageLength();
                m_queue.pop();
                return true;
            }
            return false;
        }

        
        bool FleXdLogBuffer::push(LogStream&& logStream) {
            std::lock_guard<std::mutex> bufferLock(m_mutex);
            m_sizeBuffer += logStream.getMessageLength();// TODO getMessageLength return only size of payload without the header
            if (m_sizeBuffer < m_maxSizeBuffer){
                m_queue.push(std::move(logStream));
                
                return true;
            }else {
                m_sizeBuffer -= logStream.getMessageLength();
            }
            return false;
        }




    } // namespace FlexLogger
} // namespace flexd
