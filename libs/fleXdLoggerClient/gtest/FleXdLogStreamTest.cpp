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
 * File:   FleXdLogStreamTest.cpp
 *
 * Author: Jakub Pekar
 *
 * Created on May 15, 2018, 14:12 AM
 */

#include <gtest/gtest.h>
#include "FleXdLogStream.h"

    std::string testLog("This is log message");
    flexd::logger::LogStream stream(32,15151515000,flexd::logger::MsgType::Enum::ERROR, 128, testLog);
    
namespace{
    TEST(LogStream, testGetAppID){
        EXPECT_EQ(stream.getAppID(), 32);
    }
    
    TEST(LogStream, testGetTime){
        EXPECT_EQ(stream.getTime(), 15151515000);
    }
    
    TEST(LogStream, testGetMessageType){
        EXPECT_EQ(stream.getMessageType(), flexd::logger::MsgType::Enum::ERROR);
    }
    
    TEST(LogStream, testGetMessageCounter){
        EXPECT_EQ(stream.getMessageCounter(), 128);
    }
    
    TEST(LogStream, testGetMessageLength){
        EXPECT_EQ(stream.getMessageLength(), testLog.size());
    }
    
    TEST(LogStream, testGetMessage){
        EXPECT_EQ(stream.getMessage(), testLog);
    }
    
}