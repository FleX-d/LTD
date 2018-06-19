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
 * File:   FleXdApplicationTest.cpp
 *
 * Author: Jakub Pekar
 *
 * Created on May 15, 2018, 09:02 AM
 */

#include <gtest/gtest.h>
#include "FleXdApplication.h"

flexd::logger::FleXdApplication application("TestApp",6);

TEST(Application,setState){
    EXPECT_TRUE(application.isOnline());
    application.setOffline();
    EXPECT_FALSE(application.isOnline());
    application.setOnline();
    EXPECT_TRUE(application.isOnline());
}

TEST(Application,ApplicationDescriptor){
    EXPECT_EQ(6,application.getAppDescriptor());
    application.setAppDescriptor(10);
    EXPECT_NE(6,application.getAppDescriptor());
    EXPECT_EQ(10,application.getAppDescriptor());
}

TEST(Application,LogLevel){
    EXPECT_EQ(flexd::logger::MsgType::Enum::VERBOSE,application.getLogLevel());
    application.setLogLevel(flexd::logger::MsgType::Enum::FATAL);
    EXPECT_NE(flexd::logger::MsgType::Enum::VERBOSE,application.getLogLevel());
    EXPECT_EQ(flexd::logger::MsgType::Enum::FATAL,application.getLogLevel());
}

TEST(Application, CompareName){
    EXPECT_TRUE(application.compareName("TestApp"));
}
