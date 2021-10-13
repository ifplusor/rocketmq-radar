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
#ifndef ROCKETMQ_LOGGING_LOGGER_HPP_
#define ROCKETMQ_LOGGING_LOGGER_HPP_

#include <memory>   // std::unique_ptr
#include <utility>  // std::forward, std::move

#include "logging/LogLevel.hpp"
#include "logging/LoggerConfig.hpp"
#include "utility/SourceLocation.hpp"
#include "utility/memory/MakeUnique.hpp"

namespace rocketmq {

template <typename LoggerImpl>
class Logger {
 public:
  Logger(LoggerConfig config) : logger_impl_(std::move(config)) {}

  template <typename... Args>
  Logger(Args... args) : logger_impl_(std::forward<Args>(args)...) {}

  bool ShouldLog(LogLevel level) { return logger_impl_.ShouldLog(level); }

  template <LogLevel level, typename FormatString, typename... Args>
  void Log(const SourceLocation& location, FormatString&& format, Args&&... args) {
    logger_impl_.template Log<level>(location, std::forward<FormatString>(format), std::forward<Args>(args)...);
  }

  template <typename FormatString, typename... Args>
  void Trace(const SourceLocation& location, FormatString&& format, Args&&... args) {
    Log<LogLevel::kTrace>(location, std::forward<FormatString>(format), std::forward<Args>(args)...);
  }

  template <typename FormatString, typename... Args>
  void Debug(const SourceLocation& location, FormatString&& format, Args&&... args) {
    Log<LogLevel::kDebug>(location, std::forward<FormatString>(format), std::forward<Args>(args)...);
  }

  template <typename FormatString, typename... Args>
  void Info(const SourceLocation& location, FormatString&& format, Args&&... args) {
    Log<LogLevel::kInfo>(location, std::forward<FormatString>(format), std::forward<Args>(args)...);
  }

  template <typename FormatString, typename... Args>
  void Warn(const SourceLocation& location, FormatString&& format, Args&&... args) {
    Log<LogLevel::kWarn>(location, std::forward<FormatString>(format), std::forward<Args>(args)...);
  }

  template <typename FormatString, typename... Args>
  void Error(const SourceLocation& location, FormatString&& format, Args&&... args) {
    Log<LogLevel::kError>(location, std::forward<FormatString>(format), std::forward<Args>(args)...);
  }

  template <typename FormatString, typename... Args>
  void Fatal(const SourceLocation& location, FormatString&& format, Args&&... args) {
    Log<LogLevel::kFatal>(location, std::forward<FormatString>(format), std::forward<Args>(args)...);
  }

  template <LogLevel level, typename FormatString, typename... Args>
  void Printf(const SourceLocation& location, FormatString&& format, Args&&... args) {
    logger_impl_.template Printf<level>(location, std::forward<FormatString>(format), std::forward<Args>(args)...);
  }

  template <typename FormatString, typename... Args>
  void TracePrintf(const SourceLocation& location, FormatString&& format, Args&&... args) {
    Printf<LogLevel::kTrace>(location, std::forward<FormatString>(format), std::forward<Args>(args)...);
  }

  template <typename FormatString, typename... Args>
  void DebugPrintf(const SourceLocation& location, FormatString&& format, Args&&... args) {
    Printf<LogLevel::kDebug>(location, std::forward<FormatString>(format), std::forward<Args>(args)...);
  }

  template <typename FormatString, typename... Args>
  void InfoPrintf(const SourceLocation& location, FormatString&& format, Args&&... args) {
    Printf<LogLevel::kInfo>(location, std::forward<FormatString>(format), std::forward<Args>(args)...);
  }

  template <typename FormatString, typename... Args>
  void WarnPrintf(const SourceLocation& location, FormatString&& format, Args&&... args) {
    Printf<LogLevel::kWarn>(location, std::forward<FormatString>(format), std::forward<Args>(args)...);
  }

  template <typename FormatString, typename... Args>
  void ErrorPrintf(const SourceLocation& location, FormatString&& format, Args&&... args) {
    Printf<LogLevel::kError>(location, std::forward<FormatString>(format), std::forward<Args>(args)...);
  }

  template <typename FormatString, typename... Args>
  void FatalPrintf(const SourceLocation& location, FormatString&& format, Args&&... args) {
    Printf<LogLevel::kFatal>(location, std::forward<FormatString>(format), std::forward<Args>(args)...);
  }

 private:
  LoggerImpl logger_impl_;
};

}  // namespace rocketmq

#endif  // ROCKETMQ_LOGGING_LOGGER_HPP_
