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
#ifndef ROCKETMQ_UTILITY_SOURCELOCATION_HPP_
#define ROCKETMQ_UTILITY_SOURCELOCATION_HPP_

#include <cstdint>  // int32_t

namespace rocketmq {

struct SourceLocation {
  int32_t line{0};
  int32_t column{0};
  const char* file_name{""};
  const char* function_name{""};

  SourceLocation() = default;
  SourceLocation(int32_t line, const char* file_name, const char* function_name)
      : line(line), file_name(file_name), function_name(function_name) {}
};

}  // namespace rocketmq

#define CurrentSourceLocation() \
  ::rocketmq::SourceLocation { __LINE__, __FILE__, static_cast<const char*>(__FUNCTION__) }

#endif  // ROCKETMQ_UTILITY_SOURCELOCATION_HPP_
