#include "networking/libevent/BufferEvent.hpp"

#include <event2/bufferevent.h>

#include "logging/Logging.hpp"
#include "networking/SocketUtil.h"

namespace rocketmq {

BufferEvent::BufferEvent(bufferevent* event, bool unlock_callbacks, EventLoop& loop)
    : event_loop_(loop), buffer_event_(event), unlock_callbacks_(unlock_callbacks) {
  if (buffer_event_ != nullptr) {
    bufferevent_incref(buffer_event_);
  }
}

BufferEvent::~BufferEvent() {
  if (buffer_event_ != nullptr) {
    bufferevent_decref(buffer_event_);
  }
}

void BufferEvent::SetCallback(DataCallback read_callback, DataCallback write_callback, EventCallback event_callback) {
  if (buffer_event_ == nullptr) {
    return;
  }

  // use lock in bufferevent
  bufferevent_lock(buffer_event_);

  // wrap callback
  read_callback_ = std::move(read_callback);
  write_callback_ = std::move(write_callback);
  event_callback_ = std::move(event_callback);

  bufferevent_data_cb read_cb = read_callback != nullptr ? ReadCallbackImpl : nullptr;
  bufferevent_data_cb write_cb = write_callback != nullptr ? WriteCallbackImpl : nullptr;
  bufferevent_event_cb event_cb = event_callback != nullptr ? EventCallbackImpl : nullptr;

  bufferevent_setcb(buffer_event_, read_cb, write_cb, event_cb, this);

  bufferevent_unlock(buffer_event_);
}

void BufferEvent::ReadCallbackImpl(bufferevent* bev, void* ctx) {
  auto* event = static_cast<BufferEvent*>(ctx);

  if (event->unlock_callbacks_) {
    bufferevent_lock(event->buffer_event_);
  }

  auto& callback = event->read_callback_;

  if (event->unlock_callbacks_) {
    bufferevent_unlock(event->buffer_event_);
  }

  if (callback != nullptr) {
    callback(*event);
  }
}

void BufferEvent::WriteCallbackImpl(bufferevent* bev, void* ctx) {
  auto* event = static_cast<BufferEvent*>(ctx);

  if (event->unlock_callbacks_) {
    bufferevent_lock(event->buffer_event_);
  }

  auto& callback = event->write_callback_;

  if (event->unlock_callbacks_) {
    bufferevent_unlock(event->buffer_event_);
  }

  if (callback != nullptr) {
    callback(*event);
  }
}

void BufferEvent::EventCallbackImpl(bufferevent* bev, short what, void* ctx) {
  auto* event = static_cast<BufferEvent*>(ctx);

  if (event->unlock_callbacks_) {
    bufferevent_lock(event->buffer_event_);
  }

  auto& callback = event->event_callback_;

  if (event->unlock_callbacks_) {
    bufferevent_unlock(event->buffer_event_);
  }

  if (callback != nullptr) {
    callback(*event, what);
  }
}

int BufferEvent::Connect(const std::string& address) {
  if (buffer_event_ == nullptr) {
    LOG_WARN("have not bufferevent object to connect {}", address);
    return -1;
  }

  try {
    auto* sa = StringToSockaddr(address);  // resolve domain
    peer_address_ = SockaddrToString(sa);
    return bufferevent_socket_connect(buffer_event_, sa, static_cast<int>(SockaddrSize(sa)));
  } catch (const std::exception& e) {
    LOG_ERROR("can not connect to {}, {}", address, e.what());
    return -1;
  }
}

void BufferEvent::Close() {
  if (buffer_event_ != nullptr) {
    bufferevent_free(buffer_event_);
  }
}

}  // namespace rocketmq
