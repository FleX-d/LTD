/*
Copyright (c) 2018, Globallogic s.r.o.
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
 * File:   FleXdLoggerDlt.h
 * Author: Martin Strenger
 */

#ifndef FLEXDLOGGERDLT_H
#define FLEXDLOGGERDLT_H

#include <dlt.h>
#include <map>
#include <memory>
#include <string>
#include "FleXdMessageType.h"

namespace flexd {
    namespace logger {

        struct FleXdLoggerDltData {

            explicit FleXdLoggerDltData(const std::string& description, std::unique_ptr<DltContext> context)
            : description(description), context(std::move(context)) {}

            FleXdLoggerDltData(FleXdLoggerDltData&& other)
            : description(std::move(other.description)), context(std::move(other.context)) { }

            FleXdLoggerDltData& operator=(FleXdLoggerDltData&& other) {
                description = std::move(other.description);
                context = std::move(other.context);
                return *this;
            };

            FleXdLoggerDltData(const FleXdLoggerDltData&) = delete;
            FleXdLoggerDltData& operator=(const FleXdLoggerDltData&) = delete;

            std::string description;
            std::unique_ptr<DltContext> context;
        };

        class FleXdLoggerDlt final {

        public:
            FleXdLoggerDlt();
            ~FleXdLoggerDlt();

            /**
            * Function registers the given application in dlt-daemon (as a context).
            * @param appid - unique application identifier, max 4 chars long (not counting \0)
            * @param description - application description
            * @param logLevel - log level for the application, default VERBOSE
            * @return true if registration is done without errors, false otherwise
            */
            bool registerApp(const std::string& appid, const std::string& description, MsgType::Enum logLevel = MsgType::Enum::VERBOSE);
            /**
            * Function unregisters the given application from dlt-daemon.
            * @param appid - unique application identifier, max 4 chars long (not counting \0)
            * @return true if unregistration is done without errors, false otherwise
            */
            bool unregisterApp(const std::string& appid);
            /**
            * Function sets log level for the given application.
            * @param appid - unique application identifier, max 4 chars long (not counting \0)
            * @param logLevel - new log level for the application
            * @return true if change is done without errors, false otherwise
            */
            bool setLogLevel(const std::string& appid, MsgType::Enum logLevel);
            /**
            * Function gives back status, if the given application is already registered.
            * @param appid - unique application identifier, max 4 chars long (not counting \0)
            * @return true if application is registered, false otherwise
            */
            bool isRegistered(const std::string& appid);
            /**
            * Function sends a log message to the dlt-daemon.
            * @param appid - unique application identifier of registered application
            * @param logLevel - log level for the message
            * @param log - log message
            * @return true if sending is done without errors, false otherwise
            */
            bool sendLog(const std::string& appid, MsgType::Enum logLevel, const std::string& log);

            FleXdLoggerDlt(const FleXdLoggerDlt&) = delete;
            FleXdLoggerDlt& operator=(const FleXdLoggerDlt&) = delete;

        private:
            inline bool checkIdLength(const std::string& appid) { return appid.length() > 0 && appid.length() <= DLT_ID_SIZE; }
            void initialize();

        private:
            bool m_initialized;
            std::map<std::string, FleXdLoggerDltData> m_registeredApps;
        };

    } // namespace logger
} // namespace flexd

#endif /* FLEXDLOGGERDLT_H */

