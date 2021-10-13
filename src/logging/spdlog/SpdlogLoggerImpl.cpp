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
#include "logging/spdlog/SpdlogLoggerImpl.hpp"

#include <iostream>  // std::cerr, std::endl
#include <mutex>     // std::call_once, std::once_flag
#include <utility>   // std::move

#include <spdlog/spdlog.h>
#if SPDLOG_VER_MAJOR >= 1
#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#else
#include <spdlog/async_logger.h>
#endif

namespace rocketmq {

namespace {

bool skip_config_spdlog = false;
std::once_flag config_spdlog_flag;

void ConfigSpdlog() {
  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [thread@%t] - %v [%@ (%!)]");
  // when an error occurred, flush disk immediately
  spdlog::flush_on(spdlog::level::err);
#if SPDLOG_VER_MAJOR >= 1
  spdlog::init_thread_pool(spdlog::details::default_async_q_size, 1);
  spdlog::flush_every(std::chrono::seconds(3));
#else
  spdlog::set_async_mode(8192, async_overflow_policy::block_retry, nullptr, std::chrono::milliseconds(3000), nullptr);
#endif
}

inline std::shared_ptr<spdlog::logger> CreateRotatingLogger(std::string name,
                                                            std::string filepath,
                                                            std::size_t max_size,
                                                            std::size_t max_files) {
#if SPDLOG_VER_MAJOR >= 1
  return spdlog::create_async<spdlog::sinks::rotating_file_sink_mt>(std::move(name), std::move(filepath), max_size,
                                                                    max_files);
#else
  return spdlog::rotating_logger_mt(std::move(name), std::move(filepath), max_size, max_files);
#endif
}

}  // namespace

void SkipConfigSpdlog() {
  skip_config_spdlog = true;
}

SpdlogLoggerImpl::SpdlogLoggerImpl(LoggerConfig config) {
  std::call_once(config_spdlog_flag, []() {
    if (!skip_config_spdlog) {
      ConfigSpdlog();
    }
  });

  try {
    logger_ = CreateRotatingLogger(std::move(config.name), std::move(config.path), config.file_size, config.file_nums);
    logger_->set_level(ConvertLevel(config.level));
  } catch (std::exception& e) {
    std::cerr << "initialite logger failed" << std::endl;
    exit(-1);
  }
}

SpdlogLoggerImpl::~SpdlogLoggerImpl() {
  if (own_logger_ && logger_ != nullptr) {
    spdlog::drop(logger_->name());
  }
}

}  // namespace rocketmq
