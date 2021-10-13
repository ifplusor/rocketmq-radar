/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef ROCKETMQ_LOG_LOGGING_H_
#define ROCKETMQ_LOG_LOGGING_H_

#include "logging/Logger.hpp"
#include "logging/LoggerConfig.hpp"
#include "logging/spdlog/SpdlogLoggerImpl.hpp"
#include "utility/SourceLocation.hpp"

namespace rocketmq {

LoggerConfig& GetDefaultLoggerConfig();
Logger<SpdlogLoggerImpl>& GetDefaultLogger();

}  // namespace rocketmq

#define LOG_TEMPLATE(level, suffix, ...)                          \
  do {                                                            \
    auto& logger = ::rocketmq::GetDefaultLogger();                \
    if (logger.ShouldLog(LogLevel::k##level)) {                   \
      logger.level##suffix(CurrentSourceLocation(), __VA_ARGS__); \
    }                                                             \
  } while (false)

#define LOG_FATAL(...) LOG_TEMPLATE(Fatal, , __VA_ARGS__)
#define LOG_ERROR(...) LOG_TEMPLATE(Error, , __VA_ARGS__)
#define LOG_WARN(...) LOG_TEMPLATE(Warn, , __VA_ARGS__)
#define LOG_INFO(...) LOG_TEMPLATE(Info, , __VA_ARGS__)
#define LOG_DEBUG(...) LOG_TEMPLATE(Debug, , __VA_ARGS__)

#define LOG_FATALF(...) LOG_TEMPLATE(Fatal, Printf, __VA_ARGS__)
#define LOG_ERRORF(...) LOG_TEMPLATE(Error, Printf, __VA_ARGS__)
#define LOG_WARNF(...) LOG_TEMPLATE(Warn, Printf, __VA_ARGS__)
#define LOG_INFOF(...) LOG_TEMPLATE(Info, Printf, __VA_ARGS__)
#define LOG_DEBUGF(...) LOG_TEMPLATE(Debug, Printf, __VA_ARGS__)

#endif  // ROCKETMQ_LOG_LOGGING_H_
