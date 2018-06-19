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
 * File:   FleXdLogBufferTest.cpp
 *
 * Author: Jakub Pekar
 *
 * Created on May 15, 2018, 12:02 AM
 */

#include <gtest/gtest.h>
#include "FleXdLogBuffer.h"
#include "FleXdLogStream.h"

flexd::logger::FleXdLogBuffer clientBuffer(128);    //max size of buffer in bytes
flexd::logger::FleXdLogBuffer clientBuffer2(128);
flexd::logger::FleXdLogBuffer clientBuffer3(128);

namespace {

    TEST(ClientBuffer, pushToTheBuffer){
        std::string log("This is log message");
        flexd::logger::LogData data = { 15151515000, flexd::logger::MsgType::Enum::ERROR, 128, log };

        EXPECT_TRUE(clientBuffer.push(std::move(data)));
    }

    TEST(ClientBuffer,getStreamFromTheBuffer_comparingOfStream){
        std::string log("This is log message");
        const flexd::logger::LogData* data = clientBuffer.getData();

        EXPECT_EQ(data->time, 15151515000);
        EXPECT_EQ(data->messageType, flexd::logger::MsgType::Enum::ERROR);
        EXPECT_EQ(data->messageCounter, 128);
        EXPECT_EQ(data->message, log);
    }

    TEST(CLientBuffer,removeStreamFromBuffer){
        EXPECT_TRUE(clientBuffer.pop());
        EXPECT_FALSE(clientBuffer.pop());
    }

    /* More complex tests*/

    TEST(CLientBuffer,countingOfMessageInBuffer){
        std::string log("This is log message");
        flexd::logger::LogData data1 = { 15151515000, flexd::logger::MsgType::Enum::ERROR, 128, log };
        std::string log2("This is second log message");
        flexd::logger::LogData data2 = { 15151515111, flexd::logger::MsgType::Enum::ERROR, 128, log2 };
        std::string log3("This is third log message");
        flexd::logger::LogData data3 = { 15151515222, flexd::logger::MsgType::Enum::ERROR, 128, log3 };

        EXPECT_TRUE(clientBuffer2.push(std::move(data1)));
        EXPECT_TRUE(clientBuffer2.push(std::move(data2)));
        EXPECT_TRUE(clientBuffer2.push(std::move(data3)));
        uint32_t bufferSize = log.size() + log2.size() + log3.size();
        EXPECT_EQ(bufferSize,clientBuffer2.getSizeBuffer());

        bufferSize -= log.size();
        EXPECT_TRUE(clientBuffer2.pop());
        EXPECT_EQ(bufferSize,clientBuffer2.getSizeBuffer());

        bufferSize -= log2.size();
        EXPECT_TRUE(clientBuffer2.pop());
        EXPECT_EQ(bufferSize,clientBuffer2.getSizeBuffer());

        bufferSize -= log3.size();
        EXPECT_TRUE(clientBuffer2.pop());
        EXPECT_EQ(bufferSize,clientBuffer2.getSizeBuffer());

        EXPECT_FALSE(clientBuffer2.pop());
    }

    TEST(ClientBuffer,OverFlowIdentification){

        for(int i = 0; i < 5; i++){
            std::string log("This is test log message!");
            flexd::logger::LogData data = { 15151515000, flexd::logger::MsgType::Enum::ERROR, 128, log };
            EXPECT_TRUE(clientBuffer3.push(std::move(data)));
        }
        // Buffer is full
        std::string log2("This is test log message!");
        flexd::logger::LogData data = { 15151515000, flexd::logger::MsgType::Enum::ERROR, 128, log2 };
        EXPECT_FALSE(clientBuffer3.push(std::move(data)));
    }

}
