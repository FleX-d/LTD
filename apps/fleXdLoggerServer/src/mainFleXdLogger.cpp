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
 * File:   mainFleXdLogger.cpp
 * Author: Jakub Pekar
 */

#include <signal.h>
#include <memory>
#include "FleXdLoggerServer.h"

#define CMD_LOG_TO_DLT "-dlt"


void signalHandler(int s)
{
    // correct exit, loggerServer will be eventually unregistered from Dlt
    exit(1);
}

int main(int argc, char** argv) {
    int clientDesc;
    std::unique_ptr<flexd::logger::FleXdLoggerServer> loggerServer;
    bool logTodlt = false;
    bool argError = false;

    // signal handling
    signal(SIGABRT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);

    for (int i = 1; i < argc; ++i) { // Remember argv[0] is the path to the program, we want from argv[1] onwards
        // TODO: Check argument duplicity
        if (std::string(argv[i]) == CMD_LOG_TO_DLT) {
            logTodlt = true;
        }
        //else if {
        //    TODO: other future arguments
        //}
        else
        {
            argError = true;
        }
    }

    if (argc > 1 && argError) {
        std::cerr << "Usage: " << argv[0] << " [" CMD_LOG_TO_DLT "]" << std::endl;
        return 1;
    }

    // Create server
    loggerServer = std::make_unique<flexd::logger::FleXdLoggerServer>(logTodlt);

    while(true){
        clientDesc = loggerServer->handshake();
        if(clientDesc > 0){
            loggerServer->loggingFunc(clientDesc);
    
        }
    }
    return 0;
}
