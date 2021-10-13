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
#include "EventLoop.h"

#include <memory>

#include <event2/thread.h>

#include "logging/Logging.hpp"
#include "networking/SocketUtil.h"

namespace rocketmq {

EventLoop* EventLoop::GetDefaultEventLoop() {
  static EventLoop defaultEventLoop;
  return &defaultEventLoop;
}

EventLoop::EventLoop(const struct event_config* config, bool run_immediately) {
// tell libevent support multi-threads
#ifdef WIN32
  evthread_use_windows_threads();
#else
  evthread_use_pthreads();
#endif

  if (config == nullptr) {
    event_base_ = event_base_new();
  } else {
    event_base_ = event_base_new_with_config(config);
  }

  if (event_base_ == nullptr) {
    // FIXME: failure...
    // LOG_ERROR_NEW("[CRITICAL] Failed to create event base!");
    exit(-1);
  }

  evthread_make_base_notifiable(event_base_);

  loop_thread_.SetTarget(&EventLoop::RunLoop, this);

  if (run_immediately) {
    Start();
  }
}

EventLoop::~EventLoop() {
  Stop();

  if (event_base_ != nullptr) {
    event_base_free(event_base_);
    event_base_ = nullptr;
  }
}

void EventLoop::Start() {
  if (!running_) {
    running_ = true;
    loop_thread_.Start();
  }
}

void EventLoop::Stop() {
  if (running_) {
    running_ = false;
    loop_thread_.Join();
  }
}

void EventLoop::RunLoop() {
  while (running_) {
    int ret = event_base_dispatch(event_base_);
    if (ret == 1) {
      // no event
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }
}

namespace {

constexpr int kOptionUnlockCallbacks = BEV_OPT_DEFER_CALLBACKS | BEV_OPT_UNLOCK_CALLBACKS;

}

std::unique_ptr<BufferEvent> EventLoop::CreateBufferEvent(socket_t fd, int options) {
  struct bufferevent* event = bufferevent_socket_new(event_base_, fd, options);
  if (event == nullptr) {
    auto ev_errno = EVUTIL_SOCKET_ERROR();
    // LOG_ERROR_NEW("create bufferevent failed: {}", evutil_socket_error_to_string(ev_errno));
    return nullptr;
  }

  bool unlock = (options & kOptionUnlockCallbacks) == kOptionUnlockCallbacks;

  return std::unique_ptr<BufferEvent>{new BufferEvent(event, unlock, *this)};
}

}  // namespace rocketmq
