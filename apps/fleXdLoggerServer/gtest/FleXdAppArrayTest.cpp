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
 * File:   FleXdAppArrayTest.cpp
 *
 * Author: Jakub Pekar
 *
 * Created on May 10, 2018, 12:02 AM
 */

#include <gtest/gtest.h>
#include <iostream>
#include <cstdlib>
#include "FleXdAppArray.h"
#include "FleXdApplication.h"

namespace {
    flexd::logger::FleXdAppArray array;
    flexd::logger::FleXdAppArray array2;
    flexd::logger::FleXdAppArray arrayComplex;
    flexd::logger::FleXdApplication application("Second",6);
    std::shared_ptr<flexd::logger::FleXdApplication> pApp;

    /* Base tests of functionality */

    TEST(Array,InsertToArray) {
        EXPECT_EQ(1, array.insertToArray("App",5));
        EXPECT_EQ(2, array.insertToArray("Second",6));
        EXPECT_EQ(-1, array.insertToArray("Second",7));
        EXPECT_EQ(3, array.insertToArray("reconnection",8));

        pApp = array.getApp("Second");
    }


    TEST(Array,ReturnAppNameFromArray){
        EXPECT_STREQ("Second",array.getAppName(2).c_str());
    }

    TEST(Array,AppReferenceReturn){
        EXPECT_EQ(pApp->getAppDescriptor(),application.getAppDescriptor());
        EXPECT_EQ(pApp->getLogLevel(),application.getLogLevel());
    }

    TEST(Array,UnconnectApplication){
        EXPECT_TRUE(array.disconnectApplication(6));
        EXPECT_FALSE(array.getApp("Second")->isOnline());
    }

    TEST(Array,RemovingApplicationFromArray){
        EXPECT_FALSE(array.removeFromArray(10));
        EXPECT_TRUE(array.removeFromArray(6));
    }
    /** More complex tests of functionality **/

    TEST(Array,TestOfOverFlowing){
        std::string appName;
        uint32_t maxCountOfApp = 1000;
        int appDescriptor = 5;
        std::string appString("app");

        for(uint32_t countOfApp = 1; countOfApp < maxCountOfApp; countOfApp++){
            appName = appString + std::to_string(countOfApp);
            EXPECT_EQ(countOfApp, arrayComplex.insertToArray(appName,appDescriptor));
            appDescriptor++;
        }

    }

    TEST(Array,ReconnectionOfApp){
        EXPECT_EQ(1, array2.insertToArray("App",5));
        EXPECT_EQ(2, array2.insertToArray("App1",6));
        EXPECT_EQ(3, array2.insertToArray("reconnection",7));
        EXPECT_TRUE(array2.disconnectApplication(7));
        EXPECT_FALSE(array2.getApp("reconnection")->isOnline());
        EXPECT_EQ(4, array2.insertToArray("appTest",11));
        EXPECT_EQ(3, array2.insertToArray("reconnection",11));
        EXPECT_TRUE(array2.getApp("reconnection")->isOnline());
    }
}
