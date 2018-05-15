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
 * File:   FleXdLogMessageTest.cpp
 *
 * Author: Jakub Pekar
 *
 * Created on May 15, 2018, 09:22 AM
 */

#include <gtest/gtest.h>
#include "FleXdLogMessage.h"
#include "BitStream.h"
namespace {
    std::string log("This is LOG from application");
    /*                                  appID,msgType,msgCounter,logMessage  */
    flexd::logger::FleXdLogMessage logmessage(10,4,11,log); 
    
    TEST(LogMessage, getFunctions) {
        EXPECT_EQ(10,logmessage.getAppID());
        EXPECT_NE(0,logmessage.getTime());
        EXPECT_EQ(4,logmessage.getMsgType());
        EXPECT_EQ(11,logmessage.getMsgCounter());
        EXPECT_EQ(log.size(),logmessage.getMsgSize());
        EXPECT_EQ(log,logmessage.getLogMessage());
    }
}