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
#ifndef ROCKETMQ_UTILITY_FILESYSTEM_HOMEDIRECTORY_HPP_
#define ROCKETMQ_UTILITY_FILESYSTEM_HOMEDIRECTORY_HPP_

#include <cstdlib>

#ifndef _WIN32
#include <pwd.h>
#endif

#include "utility/filesystem/Filesystem.hpp"

namespace rocketmq {
namespace Filesystem {

fs::path GetHomeDirectory() {
#ifndef WIN32
  const char* home_env = std::getenv("HOME");
  if (home_env == nullptr) {
    return getpwuid(getuid())->pw_dir;
  }
  return home_env;
#else
  return std::getenv("USERPROFILE");
#endif
}

}  // namespace Filesystem
}  // namespace rocketmq

#endif  // ROCKETMQ_UTILITY_FILESYSTEM_HOMEDIRECTORY_HPP_
