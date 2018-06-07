# Changelog
All notable changes to this project will be documented in this file.

## [Unreleased]
## [0.0.1] - 2018-06-07
### Added
- None

### Changed
- Updated Changelog [Martin Strenger]
- Finalised optional setting off dlt logging [Martin Strenger]
- Renamed class FleXdLoggerIPC to FleXdLoggerIPCServer [Martin Strenger]

### Removed
- None

## [0.0.1] - 2018-06-06
### Added
- Implementation of ICL to functional version [Jakub Pekar]

### Changed
- relocated the LogMessage(LogStream) to the common project for client and server -> removing of duplicates [Jakub Pekar]

### Removed
- Sockets [Jakub Pekar]
- LogMessage [Jakub Pekar]

## [0.0.1] - 2018-05-15
### Added
- gtest for the server application [Jakub Pekar]
- created CMake for tests [Jakub Pekar]

### Changed
- None

### Removed
- None

## [0.0.1] - 2018-05-03
### Added
- None

### Changed
- implementation of changes in BitStream [Jakub Pekar]

### Removed
- None

## [0.0.1] - 2018-05-03
### Added
- Implementation od logging to Dlt [Martin Strenger]

### Changed
- Dividin function in class object to more functions [Martin Strenger]
-repairing some bugs in code in Socket class and FleXdLoggerServer class [Martin Strenger]

### Removed
- None

## [0.0.1] - 2018-04-27
### Added
- None

### Changed
- Dividing function in class object to more functions [Jakub Pekar]
-repairing some bugs in code in Socket class and FleXdLoggerServer class [Jakub Pekar]

### Removed
- None

## [0.0.1] - 2018-04-26
### Added
- None

### Changed
- Rollback of FleXdLogger.h (FleXdLoggerClient.h -> FleXdLogger.h) [Martin Strenger]
- Corrected CMakeLists [Martin Strenger]
- Corrected comments [Martin Strenger]

### Removed
- Removed FleXdLoggerClient.h (duplicate of FleXdLogger.h) from FleXdLoggerClient [Martin Strenger]
- Removed main.cpp from FleXdLoggerClient [Martin Strenger]

## [0.0.1] - 2018-04-24
### Added
- None

### Changed
- Renamed header and source files [Martin Strenger]
- Corrected CMakeLists [Martin Strenger]
- Renamed classes and namespaces [Martin Strenger]
- Resolved compilation warnings [Martin Strenger]

### Removed
- None

## [0.0.1] - 2018-04-20
### Added
- Creating Connector class which will be implementation of IPC on LoggerServer[Jakub Pekar]
- Adding functions to Logger object for future connection on th DLT Logger.

### Changed
- Dividing complicated large functions into small functions

### Removed
- Makefile which was unknowingly pushed to GitHub

## [0.0.1] - 2018-04-13
### Added
- Creating own libraries MessageType for sharing an enum between client and server application and adding Cmake for that[Jakub Pekar]

### Changed
- None

### Removed
- MessageType from server.

## [0.0.1] - 2018-04-10
### Added
- . Adding functionality for secure, non duplicated names of client   [Jakub Pekar]

### Changed
- Implementation vector of unique_ptr for register clients on the server instead of list [Jakub Pekar]

### Removed
- None

## [0.0.1] - 2018-01-19
### Added
- Initial version [Jakub Pekar]

### Changed
- None

### Removed
- None
