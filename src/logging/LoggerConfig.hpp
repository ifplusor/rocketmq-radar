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
#ifndef ROCKETMQ_LOGGING_LOGGERCONFIG_HPP_
#define ROCKETMQ_LOGGING_LOGGERCONFIG_HPP_

#include <string>   // std::string
#include <utility>  // std::move

#include "logging/LogLevel.hpp"

namespace rocketmq {

struct LoggerConfig {
  std::string name;
  std::string path;
  LogLevel level{LogLevel::kInfo};
  int file_size{1024 * 1024 * 100};
  int file_nums{3};

  LoggerConfig() = default;
  LoggerConfig(std::string name, std::string path) : name(std::move(name)), path(std::move(path)) {}
  LoggerConfig(std::string name, std::string path, LogLevel level, int file_size, int file_nums)
      : name(std::move(name)), path(std::move(path)), level(level), file_size(file_size), file_nums(file_nums) {}
};

}  // namespace rocketmq

#endif  // ROCKETMQ_LOGGING_LOGGERCONFIG_HPP_
