

# FleXdLoggerClient

Application for logging from other applications in FleXd project.

This application is depandent on the FleXdEpoll object, that inputs as a parameter to the initialization macro of flexd logger client.

## Example Usecase

A library is created for logging to the fleXdLoggerServer (https://github.com/FleX-d/LTD/tree/devLogger/apps/fleXdLoggerServer). 

Several macro functions are defined for sending log messages to the logging server.

## Usage

### Initialization

To use the logger, it is required to create the FleXdEpoll and to set it as a parameter of the init macro. The second parameter is the application name that describes the application on the server.
    
Initialization:

    FLEX_LOG_INIT(poller, appName)
    
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
    
The file with example can be found here: https://github.com/FleX-d/LTD/tree/devLogger/libs/fleXdLoggerClient/example 

