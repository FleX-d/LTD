

# FleXdLoggerServer

Server logging application that accepts a log message from client and write it to the Syslog. If in the main CMakeList in LTD folder is WITH_DLT ('off' by default) parameter set to 'on', the logger logs to the DLT and to the Syslog simultaneously.    

## Example Usecase

The client is implemented in the library fleXdLoggerClient -> https://github.com/FleX-d/LTD/tree/devLogger/libs/fleXdLoggerClient.

## Usage

It is required to create the FleXdEpoll and put it to the parameter of the server object.

### Initilization
    flexd::icl::ipc::FleXdEpoll poller(10);
    flexd::logger::FleXdLoggerServer(poller,logToDLT); 

### Logs form

    Between server application and client library is defined log message, with following format:

     (Byte)0               2               4               6               8
           +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     header|    appID      |                   timeStamp                   |
           +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     header|    timeStamp  |msgType|Counter|     msgSize   |  msgPayload   |
           +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    payload|                          msgPayload                           |
           +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    payload|                          msgPayload                           |
           +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       
    Log messages are sent via a communication element e.g. IPC. 
## Base information about syslog implementation in a C++ application

### Open function

    openlog(const char *ident, int option, int facility);
Parameters:
  - ident - the name of program, that can be filtering through program() in syslog-ng configuration
  - option 
  - facility
### Logging function

    syslog(int priority, const char *format, ...);
Parameters:

   - priority 

    LOG_EMERG      system is unusable
    LOG_ALERT      action must be taken immediately
    LOG_CRIT       critical conditions
    LOG_ERR        error conditions
    LOG_WARNING    warning conditions
    LOG_NOTICE     normal, but significant, condition
    LOG_INFO       informational message
    LOG_DEBUG      debug-level message
    
    
## Configuration for Syslog-ng

Following lines describe a confuguration for direct logging to the file:
    "/var/log/FleXdLogger/flexdlogger.log". 
The code has to be put at the end of the config file: 
    "/etc/syslog-ng/syslog-ng.conf"

### Configuration source

To define a destination, add a destination statement to the syslog-ng configuration file using the following syntax:
    
    destination d_flexdLogger { file("/var/log/Loggger/logger.log" create-dirs(yes) dir-perm(0644) dir-owner("dev") perm(0644)); };
    file() - destiantion driver that sets the path to the file where it will save the logs. 
    In this statement it is path: /var/log/Loggger/logger.log
To the file we can get other drivers
 
    create-dirs(yes/no[defualt])  - Enable creating non-existing directories.
    dir-perm(0744) - The premision mask of directories created by syslog (0744 -> rwxr--r--)
    dir-owner(owners name) - The owner of the directories created by syslog-ng.
    perm(0744) - The permission mask of the file if it is created by syslog-ng. (0744 -> rwxr--r--)


All incoming messages comming from system syslog and logger need to be filtered by following filter:

    filter f_flexdLogger{ program("FLEXDLOGGER"); };
    
After creating the destination statement and filter, it is required to bind the settings by statement 'log{}': 

    log { source(s_src); filter(f_flexdLogger); destination(d_flexdLogger); };

For more information about syslog-ng check   

https://syslog-ng.com/documents/html/syslog-ng-ose-latest-guides/en/syslog-ng-ose-guide-admin/html/chapter-sources.html   
    
#### Configuration Summary:
    destination d_flexdLogger { file("/var/log/Loggger/logger.log" create-dirs(yes) dir-perm(0644) dir-owner("dev") perm(0644)); };
    filter f_flexdLogger{ program("FLEXDLOGGER"); };
    log { source(s_src); filter(f_flexdLogger); destination(d_flexdLogger); };
