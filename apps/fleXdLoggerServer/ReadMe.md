

# FleXdLoggerServer

Server logging application that accept log message from client and write it to the Syslog or DLT.

## Example Usecase

The client is implement in library fleXdLoggerClient -> https://github.com/FleX-d/LTD/tree/devLogger/libs/fleXdLoggerClient.

## Usage

### Initilization

std::unique_ptr<flexd::logger::FleXdLoggerServer> loggerServer;
loggerServer = std::make_unique<flexd::logger::FleXdLoggerServer>(logToDLT);    /if parameter is true it will be loggin to DLT, else logging to syslog

### Logs form

Between server application and client library is defined log message, which has this form:

(Byte) 0               2               4               6               8
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 header|    appID      |                   timeStamp                   |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 header|    timeStamp  |msgType|Counter|     msgSize   |  msgPayload   |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
payload|                          msgPayload                           |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
payload|                          msgPayload                           |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       
The logs are sent via a communication element e.g. sockets.       