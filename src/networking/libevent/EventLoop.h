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
#ifndef ROCKETMQ_TRANSPORT_EVENTLOOP_H_
#define ROCKETMQ_TRANSPORT_EVENTLOOP_H_

#include <functional>  // std::function

#include <event2/buffer.h>
#include <event2/event.h>

#include "utility/thread/Thread.hpp"

using socket_t = evutil_socket_t;

namespace rocketmq {

class BufferEvent;

class EventLoop final {
 public:
  static EventLoop* GetDefaultEventLoop();

 public:
  explicit EventLoop(const event_config* config = nullptr, bool run_immediately = true);
  ~EventLoop();

  // disable copy
  EventLoop(const EventLoop&) = delete;
  EventLoop& operator=(const EventLoop&) = delete;

  // disable move
  EventLoop(EventLoop&&) = delete;
  EventLoop& operator=(EventLoop&&) = delete;

  void Start();
  void Stop();

  bool running() const { return running_; }

  std::unique_ptr<BufferEvent> CreateBufferEvent(socket_t fd, int options);

 private:
  void RunLoop();

 private:
  struct event_base* event_base_{nullptr};
  Thread loop_thread_{"EventLoop"};

  bool running_{false};  // aotmic is unnecessary
};


}  // namespace rocketmq

#endif  // ROCKETMQ_TRANSPORT_EVENTLOOP_H_
