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
 * File:   iSocServer.cpp
 * Author: Jakub Pekar
 */

enum LogLevelsEnum {
    HANDSHAKE = 0,
    VERBOSE = 1,
    DEBUG = 2,
    INFO = 3,
    WARN = 4,
    ERROR = 5,
    FATAL = 6,
    ALL = 7
};
#include "iSocServer.h"

iSocServer::iSocServer()
: address(),
opt(1),
addrlen(sizeof (address)),
buffer() 
{
}

bool iSocServer::connectFunck(int port) {
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return false;
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
            &opt, sizeof (opt))) {
        perror("setsockopt");
        return false;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *) &address,
            sizeof (address)) < 0) {
        perror("bind failed");
        return false;
    }
    printf("Server was created\n");
    return true;
}

bool iSocServer::listenServer() {

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        return false;
    }
    return true;
}

int iSocServer::connectClient() {
    int socket = 0;

    if ((socket = accept(server_fd, (struct sockaddr *) &address,
            (socklen_t*) & addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    return socket;
}

int iSocServer::recv(int descriptor, void* buffer, uint16_t size) {
    valread = read(descriptor, buffer, size);
    if (valread < 0) {
        std::perror("ERROR:");
        return valread;
    }
    return valread;
}

bool iSocServer::send(int descriptor, void* buffer, uint16_t size) {
    valread =write(descriptor, buffer, size);
    if (valread < 0) {
        std::perror("ERROR:");
        return valread;
    }
    return valread;
}

void iSocServer::printSocketfd() {
    std::cout << server_fd << "\n";
}

iSocServer::~iSocServer() {
    close(server_fd);
}


