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
#ifndef ROCKETMQ_UTILITY_FILESYSTEM_FILESYSTEM_HPP_
#define ROCKETMQ_UTILITY_FILESYSTEM_FILESYSTEM_HPP_

#include "rocketmq/config.hpp"

#if defined(_ROCKETMQ_LIB_FILESYSTEM)

#include <filesystem>
#include <fstream>

namespace rocketmq {

namespace fs = std::filesystem;

}  // namespace rocketmq

#else  // defined(_ROCKETMQ_LIB_FILESYSTEM)

#include <ghc/filesystem.hpp>

namespace rocketmq {

namespace fs = ::ghc::filesystem;

}  // namespace rocketmq

#endif  // defined(_ROCKETMQ_LIB_FILESYSTEM)

#endif  // ROCKETMQ_UTILITY_FILESYSTEM_FILESYSTEM_HPP_
