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
        public:
            LogData()
            : m_time(0),
              m_messageType(flexd::logger::MsgType::Enum::UNDEFINED),
              m_messageCounter(0),
              m_message() {
            }

            LogData(time_t time, flexd::logger::MsgType::Enum messageType, uint8_t messageCounter, const std::string& message)
            : m_time(time),
              m_messageType(messageType),
              m_messageCounter(messageCounter),
              m_message(message) {
            }

            LogData(const LogData& other)
            : m_time(other.m_time),
              m_messageType(other.m_messageType),
              m_messageCounter(other.m_messageCounter),
              m_message(other.m_message) {
            }

            LogData& operator=(const LogData& other) {
                if (this != &other) {
                    m_time = other.m_time;
                    m_messageType = other.m_messageType;
                    m_messageCounter = other.m_messageCounter;
                    m_message = other.m_message;
                }
                return *this;
            }

            LogData(LogData&& other)
            : m_time(other.m_time),
              m_messageType(other.m_messageType),
              m_messageCounter(other.m_messageCounter),
              m_message(other.m_message) {
                other.m_time = 0;
                other.m_messageType = flexd::logger::MsgType::Enum::UNDEFINED;
                other.m_messageCounter = 0;
                other.m_message.clear();
            }

            LogData& operator=(LogData&& other) {
                if (this != &other) {
                    m_time = other.m_time;
                    m_messageType = other.m_messageType;
                    m_messageCounter = other.m_messageCounter;
                    m_message = std::move(other.m_message);

                    other.m_time = 0;
                    other.m_messageType = flexd::logger::MsgType::Enum::UNDEFINED;
                    other.m_messageCounter = 0;
                    other.m_message.clear();
                }
                return *this;
            }

            time_t m_time;
            flexd::logger::MsgType::Enum m_messageType;
            uint8_t m_messageCounter;
            std::string m_message;
        };

        class FleXdLogBuffer {

        public:
            FleXdLogBuffer(size_t maxSizeBuffer);
            FleXdLogBuffer(const FleXdLogBuffer& orig) = delete;
            virtual ~FleXdLogBuffer() = default;
            /**
             * Function returns reference to the first element in the queue. If the queue is empty,
             * behaviour is undefined! Operation is not synchronized, use getLock() and always check
             * for size before calling this function!
             * @return reference to the first element.
             */
            const LogData& front() const;
             /**
            * Function inserts the logStream on the back queue.
            * @param logData - inserted log
            * @return true after successful inserting
            */
            bool push(LogData&& logData);
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
            /**
             * Function returns new unique lock to buffer. Does not lock.
             * @return unique lock to buffer
             */
            std::unique_lock<std::mutex> getLock();

        private:
            size_t m_maxSizeBuffer;
            size_t m_sizeBuffer;
            std::queue<LogData> m_queue;
            std::mutex m_mutex;
        };

    } // namespace FlexLogger
} // namespace flexd


#endif /* FLEXDLOGBUFFER_H */

