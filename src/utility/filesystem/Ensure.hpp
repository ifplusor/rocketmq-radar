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
#ifndef ROCKETMQ_UTILITY_FILESYSTEM_ENSURE_HPP_
#define ROCKETMQ_UTILITY_FILESYSTEM_ENSURE_HPP_

#include <cstddef>  // size_t

#include <system_error>

#include "utility/filesystem/Filesystem.hpp"

namespace rocketmq {
namespace Filesystem {

inline void EnsureDirectory(const fs::path& directory_path) {
  auto directory_status = fs::status(directory_path);
  if (fs::exists(directory_status)) {
    if (!fs::is_directory(directory_status)) {
      throw fs::filesystem_error("The path exists, but not a directory", directory_path,
                                 std::make_error_code(std::errc::file_exists));
    }
  } else {
    fs::create_directories(directory_path);
  }
}

inline void EnsureRegularFile(const fs::path& file_path) {
  auto file_status = fs::status(file_path);
  if (fs::exists(file_status)) {
    if (!fs::is_regular_file(file_status)) {
      throw fs::filesystem_error("The path exists, but not a regular file", file_path,
                                 std::make_error_code(std::errc::file_exists));
    }
  } else {
    // create file
    fs::ofstream ofs(file_path, std::ios_base::binary | std::ios_base::app);
  }
}

inline void EnsureRegularFile(const fs::path& file_path, size_t file_size) {
  EnsureRegularFile(file_path);
  fs::resize_file(file_path, file_size);
}

}  // namespace Filesystem
}  // namespace rocketmq

#endif  // ROCKETMQ_UTILITY_FILESYSTEM_ENSURE_HPP_
