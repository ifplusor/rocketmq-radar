
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
#ifndef ROCKETMQ_LOGGING_SPDLOG_SPDLOGLOGGERIMPL_HPP_
#define ROCKETMQ_LOGGING_SPDLOG_SPDLOGLOGGERIMPL_HPP_

#include <memory>   // std::shared_ptr
#include <utility>  // std::move

// clang-format off
#include <spdlog/spdlog.h>
#if !defined(SPDLOG_FMT_EXTERNAL)
#include <spdlog/fmt/bundled/printf.h>
#else
#include <fmt/printf.h>
#endif
// clang-format on

#include "logging/LogLevel.hpp"
#include "logging/LoggerConfig.hpp"
#include "utility/SourceLocation.hpp"

namespace rocketmq {

void SkipConfigSpdlog();

class SpdlogLoggerImpl final {
 public:
  SpdlogLoggerImpl(LoggerConfig config);
  SpdlogLoggerImpl(std::shared_ptr<spdlog::logger> logger, bool own_logger = true)
      : logger_(std::move(logger)), own_logger_(own_logger) {}

  ~SpdlogLoggerImpl();

  // disable copy
  SpdlogLoggerImpl(const SpdlogLoggerImpl&) = delete;
  SpdlogLoggerImpl& operator=(const SpdlogLoggerImpl&) = delete;

  // disable move
  SpdlogLoggerImpl(SpdlogLoggerImpl&&) = delete;
  SpdlogLoggerImpl& operator=(SpdlogLoggerImpl&&) = delete;

  bool ShouldLog(LogLevel level) { return logger_->should_log(ConvertLevel(level)); }

  template <LogLevel level, typename FormatString, typename... Args>
  void Log(const SourceLocation& location, FormatString&& format, Args&&... args) {
    auto spdlog_level = ConvertLevel(level);
    logger_->log(ConvertLocation(location), spdlog_level, format, std::forward<Args>(args)...);
  }

  template <LogLevel level, typename FormatString, typename... Args>
  void Printf(const SourceLocation& location, FormatString&& format, Args&&... args) {
    auto spdlog_level = ConvertLevel(level);
    if (ShouldLog(level)) {
      std::string message = fmt::sprintf(format, std::forward<Args>(args)...);
      logger_->log(ConvertLocation(location), spdlog_level, message);
    }
  }

 private:
  static constexpr spdlog::level::level_enum ConvertLevel(LogLevel level) {
    return static_cast<spdlog::level::level_enum>(6 - static_cast<int>(level));
  }

  static spdlog::source_loc ConvertLocation(const SourceLocation& location) {
    return {location.file_name, location.line, location.function_name};
  }

 private:
  std::shared_ptr<spdlog::logger> logger_;
  bool own_logger_{true};
};

}  // namespace rocketmq

#endif  // ROCKETMQ_LOGGING_SPDLOG_SPDLOGLOGGERIMPL_HPP_
