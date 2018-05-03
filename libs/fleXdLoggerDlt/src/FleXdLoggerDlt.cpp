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
 * File:   FleXdLoggerDlt.cpp
 * Author: Martin Strenger
 */

#include "FleXdLoggerDlt.h"

#define DLT_FLEXD_APPID "FLXL"
#define DLT_FLEXD_DESCRIPTION "FleXdLoggerServer"

namespace {
    DltLogLevelType msgTypeToDltLogLevel(flexd::logger::MsgType::Enum msgType) {
        switch (msgType)
        {
            case flexd::logger::MsgType::Enum::VERBOSE:
                return DLT_LOG_VERBOSE;
            case flexd::logger::MsgType::Enum::DEBUG:
                return DLT_LOG_DEBUG;
            case flexd::logger::MsgType::Enum::INFO:
                return DLT_LOG_INFO;
            case flexd::logger::MsgType::Enum::WARN:
                return DLT_LOG_WARN;
            case flexd::logger::MsgType::Enum::ERROR:
                return DLT_LOG_ERROR;
            case flexd::logger::MsgType::Enum::FATAL:
                return DLT_LOG_FATAL;
            default:
                return DLT_LOG_DEFAULT;
        }
    }
}

namespace flexd {
    namespace logger {

        FleXdLoggerDlt::FleXdLoggerDlt()
        : m_initialized(false) {
            // Try to register dlt_application
            initialize();
        }

        FleXdLoggerDlt::~FleXdLoggerDlt() {
            if (m_initialized) {
                if (!m_registeredApps.empty()) {
                    for (auto it = m_registeredApps.rbegin(); it != m_registeredApps.rend(); ++it) {
                        // Unregister dlt_context and ignore the results
                        dlt_unregister_context(it->second.context.get());
                    }
                    m_registeredApps.clear();
                }
                // Unregister dlt_application and ignore the result
                dlt_unregister_app();
            }
        }

        bool FleXdLoggerDlt::registerApp(const std::string& appid, const std::string& description, MsgType::Enum logLevel /*= MsgType::Enum::VERBOSE*/) {
            // When not successfully registered yet, try to register dlt_application again
            if (!m_initialized) {
                initialize();
            }
            if (m_initialized && checkIdLength(appid) && !isRegistered(appid) && msgTypeToDltLogLevel(logLevel) != DLT_LOG_DEFAULT) {
                std::unique_ptr<DltContext> context(new DltContext());

                // Try to register dlt_context
                if (DLT_RETURN_OK == dlt_register_context_ll_ts(context.get(), appid.c_str(), description.c_str(), msgTypeToDltLogLevel(logLevel), DLT_TRACE_STATUS_OFF)) {
                    return m_registeredApps.insert(std::make_pair(appid, std::move(FleXdLoggerDltData(description, std::move(context))))).second;
                }
            }
            return false;
        }

        bool FleXdLoggerDlt::unregisterApp(const std::string& appid) {
            if (!m_initialized || m_registeredApps.empty()) {
                return true;
            }
            if (checkIdLength(appid)) {
                auto it = m_registeredApps.find(appid);
                if (it == m_registeredApps.end()) {
                    return true;
                }

                // Try to unregister dlt_context
                if (DLT_RETURN_OK == dlt_unregister_context(it->second.context.get())) {
                    m_registeredApps.erase(it);
                    return true;
                }
            }
            return false;
        }

        bool FleXdLoggerDlt::setLogLevel(const std::string& appid, MsgType::Enum logLevel) {
            if (!m_initialized || m_registeredApps.empty() || !checkIdLength(appid)) {
                return false;
            }

            auto it = m_registeredApps.find(appid);
            if (it != m_registeredApps.end()) {
                if (*(it->second.context->log_level_ptr) != static_cast<int32_t>(msgTypeToDltLogLevel(logLevel))) {
                    if (DLT_RETURN_OK == dlt_unregister_context(it->second.context.get())) {
                        m_registeredApps.erase(it);
                        return registerApp(appid, it->second.description, logLevel);
                    }
                }
            }
            return false;
        }

        bool FleXdLoggerDlt::sendLog(const std::string& appid, MsgType::Enum logLevel, const std::string& log) {
            if (!m_initialized || m_registeredApps.empty() || !checkIdLength(appid)) {
                return false;
            }

            auto it = m_registeredApps.find(appid);
            if (it != m_registeredApps.end()) {
                if(DLT_RETURN_TRUE == dlt_user_is_logLevel_enabled(it->second.context.get(), msgTypeToDltLogLevel(logLevel))) {
                    DltContextData log_local;
                    int dlt_local = dlt_user_log_write_start(it->second.context.get(), &log_local, msgTypeToDltLogLevel(logLevel));
                    if (dlt_local > 0) {
                        if (DLT_RETURN_OK == dlt_user_log_write_string(&log_local, log.c_str())) {
                            return (DLT_RETURN_OK == dlt_user_log_write_finish(&log_local));
                        }
                    }
                }
            }
            return false;
        }

        bool FleXdLoggerDlt::isRegistered(const std::string& appid) {
            return (!m_registeredApps.empty() && m_registeredApps.find(appid) != m_registeredApps.end());
        }

        void FleXdLoggerDlt::initialize() {
            // Try to register dlt_application
            m_initialized = (DLT_RETURN_OK == dlt_register_app(DLT_FLEXD_APPID, DLT_FLEXD_DESCRIPTION));
        }

    } // namespace FlexLogger
} // namespace flexd
