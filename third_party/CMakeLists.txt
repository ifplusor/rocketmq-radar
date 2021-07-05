# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
include(FetchContent)

FetchContent_Declare(
  gsl-lite
  GIT_REPOSITORY "https://github.com/gsl-lite/gsl-lite.git"
  GIT_TAG "v0.38.1"
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(gsl-lite)

FetchContent_Declare(
  filesystem
  GIT_REPOSITORY "https://github.com/gulrak/filesystem.git"
  GIT_TAG "v1.5.8")
FetchContent_MakeAvailable(filesystem)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
  GIT_TAG "v1.8.5"
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
  jsoncpp
  GIT_REPOSITORY "https://github.com/open-source-parsers/jsoncpp.git"
  GIT_TAG "1.9.4"
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(jsoncpp)

FetchContent_Declare(
  libevent
  GIT_REPOSITORY "https://github.com/libevent/libevent.git"
  GIT_TAG "release-2.1.12-stable"
  GIT_SHALLOW TRUE)
if(NOT libevent_POPULATED)
  FetchContent_Populate(libevent)
  set(EVENT__DISABLE_OPENSSL
      ON
      CACHE INTERNAL "Build libevent without support for OpenSSL")
  set(EVENT__DISABLE_BENCHMARK
      ON
      CACHE INTERNAL "Build libevent without benchmark")
  set(EVENT__DISABLE_SAMPLES
      ON
      CACHE INTERNAL "Build libevent without samples")
  add_subdirectory(${libevent_SOURCE_DIR} ${libevent_BINARY_DIR})
endif()
