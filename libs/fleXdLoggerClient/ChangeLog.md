# Changelog
All notable changes to this project will be documented in this file.

## [Unreleased]
## [0.0.1] - 2018-07-11
### Added
- None

### Changed
- Code update regarding changes in ICL library [Martin Strenger]

### Removed
- None

## [0.0.1] - 2018-07-02
### Added
- None

### Changed
- Fixed crash when FLEX_LOG_INIT was not called [Martin Strenger]
- Code update regarding changes in ICL library [Martin Strenger]

### Removed
- None

## [0.0.1] - 2018-06-21
### Added
- Constructors and operators for structure LogData [Martin Strenger]
- New functions front() and getLock() for class FleXdLogBuffer [Martin Strenger]

### Changed
- Fixed crashing of function flushBuffer() in class FleXdLoggerIPCClient [Martin Strenger]

### Removed
- Deleted function getData() from class FleXdLogBuffer [Martin Strenger]

## [0.0.1] - 2018-06-21
### Added
- None

### Changed
- Small code improvements [Martin Strenger]

### Removed
- None

## [0.0.1] - 2018-06-12
### Added
- CMakeLists option to enable examples [Martin Strenger]

### Changed
- Fixed compile errors for unit tests [Martin Strenger]

### Removed
- None

## [0.0.1] - 2018-06-06
### Added
- implementation of ICL [Jakub Pekar]

### Changed
- relocated the LogStream to the common project for client and server -> removing of duplicates [Jakub Pekar]

### Removed
- Socket which is replaced by ICL [Jakub Pekar]
- LogStream [Jakub Pekar]

## [0.0.1] - 2018-05-15
### Added
- tests for client library [Jakub Pekar]
- google tests to CMake [Jakub Pekar]

### Changed
- None

### Removed
- None

## [0.0.1] - 2018-05-10
### Added
- Buffering for logs [Jakub Pekar]
- create ReadMe [Jakub Pekar]

### Changed
- dividing the log function to separate function writeLogToBuffer and sendLog [Jakub Pekar]
- add buffer class to cmake [Jakub Pekar]

### Removed
- None

## [0.0.1] - 2018-04-27
### Added
- None

### Changed
- Solution for offline logging to console [Jakub Pekar]
- repair bugs in Socket class [Jakub Pekar]
- modification connection to server and editing some messeges [Jakub Pekar]

### Removed
- None

## [0.0.1] - 2018-04-24
### Added
- Initial version [Jakub Pekar]

### Changed
- None

### Removed
- None
