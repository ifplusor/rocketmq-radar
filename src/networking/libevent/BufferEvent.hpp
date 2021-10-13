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
#ifndef ROCKETMQ_NETWORKING_LIBEVENT_BUFFEREVENT_HPP_
#define ROCKETMQ_NETWORKING_LIBEVENT_BUFFEREVENT_HPP_

#include <functional>
#include <string>

#include <event2/bufferevent.h>

namespace rocketmq {

class EventLoop;

class BufferEvent {
 public:
  using DataCallback = std::function<void(BufferEvent&)>;
  using EventCallback = std::function<void(BufferEvent&, short)>;

 private:
  BufferEvent(bufferevent* event, bool unlock_callbacks, EventLoop& loop);
  friend EventLoop;

 public:
  ~BufferEvent();

  // disable copy
  BufferEvent(const BufferEvent&) = delete;
  BufferEvent& operator=(const BufferEvent&) = delete;

  // disable move
  BufferEvent(BufferEvent&&) = delete;
  BufferEvent& operator=(BufferEvent&&) = delete;

  void SetCallback(DataCallback read_callback, DataCallback write_callback, EventCallback event_callback);

  void SetWatermark(short events, size_t lowmark, size_t highmark) {
    bufferevent_setwatermark(buffer_event_, events, lowmark, highmark);
  }

  int Enable(short event) { return bufferevent_enable(buffer_event_, event); }
  int Disable(short event) { return bufferevent_disable(buffer_event_, event); }

  int Connect(const std::string& address);
  void Close();

  int Write(const void* data, size_t size) { return bufferevent_write(buffer_event_, data, size); }
  size_t Read(void* data, size_t size) { return bufferevent_read(buffer_event_, data, size); }

  struct evbuffer* GetInput() {
    return bufferevent_get_input(buffer_event_);
  }

  evutil_socket_t GetFD() const { return bufferevent_getfd(buffer_event_); }

  const std::string& peer_address() const { return peer_address_; }

 private:
  static void ReadCallbackImpl(bufferevent* bev, void* ctx);
  static void WriteCallbackImpl(bufferevent* bev, void* ctx);
  static void EventCallbackImpl(bufferevent* bev, short what, void* ctx);

 private:
  EventLoop& event_loop_;
  bufferevent* buffer_event_{nullptr};
  const bool unlock_callbacks_{false};

  DataCallback read_callback_;
  DataCallback write_callback_;
  EventCallback event_callback_;

  // cached properties
  std::string peer_address_;
};

}  // namespace rocketmq

#endif  // ROCKETMQ_NETWORKING_LIBEVENT_BUFFEREVENT_HPP_
