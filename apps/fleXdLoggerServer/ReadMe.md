

# FleXdLoggerServer

Server logging application that accept log message from client and write it to the Syslog. If in the main CMakeList in LTD folder is WITH_DLT ('off' default) parameter set to the 'on', the logger will log simultaneously to the dlt and to syslog.    

## Example Usecase

The client is implement in library fleXdLoggerClient -> https://github.com/FleX-d/LTD/tree/devLogger/libs/fleXdLoggerClient.

## Usage

It is necessarily create the FleXdEpoll and put it to the parameter of the server object.

### Initilization
    flexd::icl::ipc::FleXdEpoll poller(10);
    flexd::logger::FleXdLoggerServer(poller,logToDLT); 

### Logs form

    Between server application and client library is defined log message, which has this form:

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
       
    The logs are sent via a communication element e.g. IPC 
## Base information about syslog implementation in C++ application

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

These lines describe the confuguration for direct logging to the file:
    "/var/log/FleXdLogger/flexdlogger.log". 
The code have to be put to the end of config file: 
    "/etc/syslog-ng/syslog-ng.conf"

### Configuration source

To define a destination, add a destination statement to the syslog-ng configuration file using the following syntax:
    
    destination d_flexdLogger { file("/var/log/Loggger/logger.log" create-dirs(yes) dir-perm(0644) dir-owner("dev") perm(0644)); };
    file() - Is destiantion driver that set the path to the file where i will be saving the logs. 
    In this statement si path: /var/log/Loggger/logger.log
 To the file we can get other drivers
 
    create-dirs(yes/no[defualt])  - Enable creating non-existing directories.
    dir-perm(0744) - The premision mask of directories created by syslog (0744 -> rwxr--r--)
    dir-owner(owners name) - The owner of the directories created by syslog-ng.
    perm(0744) - The permission mask of the file if it is created by syslog-ng. (0744 -> rwxr--r--)


All incoming messages are from system syslog and logger needs filtering logs from FLEXDLOGGER. For this it is defined filter:

    filter f_flexdLogger{ program("FLEXDLOGGER"); };
    
After you create the destination statement and filter, is needing bind the settings by statement 'log{}': 

    log { source(s_src); filter(f_flexdLogger); destination(d_flexdLogger); };

For more information about syslog-ng check   

https://syslog-ng.com/documents/html/syslog-ng-ose-latest-guides/en/syslog-ng-ose-guide-admin/html/chapter-sources.html   
    
#### Configuration Summary:
    destination d_flexdLogger { file("/var/log/Loggger/logger.log" create-dirs(yes) dir-perm(0644) dir-owner("dev") perm(0644)); };
    filter f_flexdLogger{ program("FLEXDLOGGER"); };
    log { source(s_src); filter(f_flexdLogger); destination(d_flexdLogger); };
