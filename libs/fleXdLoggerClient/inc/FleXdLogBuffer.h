/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   FleXdLogBuffer.h
 * Author: dev
 *
 * Created on May 3, 2018, 1:25 PM
 */

#ifndef FLEXDLOGBUFFER_H
#define FLEXDLOGBUFFER_H

#include <queue>
#include <memory>
#include <mutex>

#include "FleXdLogStream.h"

namespace flexd {
    namespace logger {
        struct LogData {
            time_t time;
            flexd::logger::MsgType::Enum messageType;
            uint8_t messageCounter;
            std::string message;
        };

        class FleXdLogBuffer {

        public:
            FleXdLogBuffer(size_t maxSizeBuffer);
            FleXdLogBuffer(const FleXdLogBuffer& orig) = delete;
            virtual ~FleXdLogBuffer() = default;
             /**
            * Function inserts the logStream on the back queue.
            * @param logStream - inserted log
            * @return true after successful inserting
            */
            bool push(LogData&& logData);
             /**
            * Function gets the first logStream from queue, but not remove it.
            * @return pointer to the first element in queue
            */
            const LogData* getData() const;
             /**
            * Function removes an element from the front of the queue.
            * @return true if removing is successful, false otherwise
            */
            bool pop();
             /**
            * Function return the count of elements in buffer.
            * @return size of buffer
            */
            uint32_t getSizeBuffer();

        private:
            size_t m_maxSizeBuffer;
            size_t m_sizeBuffer;
            std::queue<LogData> m_queue;
            std::mutex m_mutex;
        };

    } // namespace FlexLogger
} // namespace flexd


#endif /* FLEXDLOGBUFFER_H */

