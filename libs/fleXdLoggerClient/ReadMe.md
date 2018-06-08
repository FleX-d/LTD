

# FleXdLoggerClient

Application for logging from applications in FleXd project.

This application is depandent on the FleXdEpoll object, that enters as a parameter to the initialization macro of flexd logger client.

## Example Usecase

Library is created for logging to the fleXdLoggerServer (https://github.com/FleX-d/LTD/tree/devLogger/apps/fleXdLoggerServer). 
For the logging are defined the macros, that sending logs on the server.

## Usage

### Initialization

To use the logger, it is necessarily create the FleXdEpoll and set it as a parameter of the init macro. The second init macros parameter is the application name that will be describe the application on to the server.
    
Initialization:
    FLEX_LOG_INIT(poller,appName)
    
### Logging

The logger has defined 6 types of logs:
-trace
    FLEX_LOG_TRACE(log)
-debug
    FLEX_LOG_DEBUG(log)
-info
    FLEX_LOG_INFO(log)
-warning
    FLEX_LOG_WARN(log)
-error 
    FLEX_LOG_ERROR(log)
-fatal error
    FLEX_LOG_FATAL(log)
        
## Example
    
The file with example is here: https://github.com/FleX-d/LTD/tree/devLogger/libs/fleXdLoggerClient/example 

